#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QRegularExpression>

#include <private/qzipwriter_p.h>

#include <memory>

namespace KDHockeyApp {

class CollectSymbols : public QCoreApplication
{
public:
    using QCoreApplication::QCoreApplication;

    int run()
    {
        QCommandLineParser args;
        args.addOption({"dumpsyms", "PATH", "The machine specific dumpsyms binary to use"});
        args.addOption({"objdump", "PATH", "The machine specific objdump binary to use"});
        args.addOption({"library-path", "PATH", "Directories of where to find shared libraries"});
        args.addPositionalArgument("TARGET", "The file from which to collect symbols");
        args.addPositionalArgument("ARCHIVE", "The archive to create");
        args.addHelpOption();

        args.process(arguments());
        const auto pargs = args.positionalArguments();

        if (pargs.size() < 1) {
            qWarning("Invalid usage: Target missing");
            return EXIT_FAILURE;
        }

        if (pargs.size() < 2) {
            qWarning("Invalid usage: Archive missing");
            return EXIT_FAILURE;
        }

        m_dumpsyms = args.value("dumpsyms");
        m_objdump = args.value("objdump");

        if (m_dumpsyms.isEmpty())
            m_dumpsyms= "dump_syms";
        if (m_objdump.isEmpty())
            m_objdump = "objdump";

        m_libraryPath = args.values("library-path");

        if (m_libraryPath.isEmpty()) {
#ifdef Q_OS_WIN32
            m_libraryPath = QString::fromLocal8Bit(qgetenv("PATH")).split(QDir::listSeparator());
#else
            m_libraryPath = QString::fromLocal8Bit(qgetenv("LD_LIBRARY_PATH")).split(QDir::listSeparator());
#endif
        }

        const auto target = pargs.at(0);
        const auto archive = pargs.at(1);

        if (!archive.endsWith(".zip")) {
            qWarning("Invalid usage: Invalid target type");
            return EXIT_FAILURE;
        }

        m_workDir.setPath(archive.left(archive.length() - 4));
        m_workDir.removeRecursively();

        if (!m_workDir.mkpath(".")) {
            qWarning("Could not create work directory");
            return EXIT_FAILURE;
        }

        m_zipWriter = std::make_unique<QZipWriter>(archive);

        if (!dumpSymbols(target))
            return EXIT_FAILURE;

        const auto dependencyList = dependencies(target);

        if (!dependencyList)
            return EXIT_FAILURE;

        for (const auto &libraryName: *dependencyList) {
            if (!dumpSymbols(findLibrary(libraryName)))
                return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

private:
    bool waitForFinished(QProcess *process)
    {
        if (!process->waitForFinished()) {
            qWarning("%ls: %ls", qUtf16Printable(process->program()), qUtf16Printable(process->errorString()));
            return false;
        }

        if (process->exitStatus() != QProcess::NormalExit) {
            qWarning("%ls: The program has crashed.", qUtf16Printable(process->program()));
            return false;
        }

        if (process->exitCode() != EXIT_SUCCESS) {
            qWarning("%ls: The program has failed with exit code %d.",
                     qUtf16Printable(process->program()), process->exitCode());
            return false;
        }

        return true;
    }

    std::optional<QStringList> dependencies(const QFileInfo &fileInfo)
    {
        qInfo("Collecting dependencies for %ls...", qUtf16Printable(fileInfo.baseName()));

        QProcess objdump;
        objdump.setEnvironment({"LC_ALL=C"});
        objdump.setProcessChannelMode(QProcess::ForwardedErrorChannel);
        objdump.setReadChannel(QProcess::StandardOutput);
        objdump.start(m_objdump, {"-x", fileInfo.filePath()});

        if (!objdump.waitForReadyRead()) {
            qWarning("%ls: %ls", qUtf16Printable(objdump.program()), qUtf16Printable(objdump.errorString()));
            return {};
        }

        QRegularExpression pattern{R"(^\s+(?:NEEDED|DLL Name:)\s+(\S+))"};
        QStringList dependencies;

        while (!objdump.atEnd()) {
            const auto line = QString::fromLocal8Bit(objdump.readLine());
            const auto test = pattern.match(line);

            if (test.hasMatch())
                dependencies.append(test.captured(1));
        }

        if (!waitForFinished(&objdump))
            return {};

        return {dependencies};
    }

    QString findLibrary(const QString &libraryName)
    {
        for (const QDir path: m_libraryPath) {
            const auto fileName = path.filePath(libraryName);
            if (QFile::exists(fileName))
                return fileName;
        }

        return libraryName;
    }

    bool dumpSymbols(const QFileInfo &fileInfo)
    {
        qInfo("Scanning %ls...", qUtf16Printable(fileInfo.fileName()));
        const QFileInfo symbolFileInfo{m_workDir.filePath(fileInfo.fileName() + ".sym")};

        QProcess dumpsyms;

        dumpsyms.setEnvironment({"LC_ALL=C"});
        dumpsyms.setProcessChannelMode(QProcess::ForwardedErrorChannel);
        dumpsyms.setStandardOutputFile(symbolFileInfo.filePath());
        dumpsyms.start(m_dumpsyms, {fileInfo.filePath()});

        if (!waitForFinished(&dumpsyms))
            return false;

        dumpsyms.close();

        const auto symbolDir = "symbols/" + fileInfo.fileName() + "/" + moduleVersion(symbolFileInfo.filePath());
        const auto symbolFileName = symbolDir + "/" + symbolFileInfo.fileName();

        QFile symbolFile{symbolFileInfo.filePath()};

        if (!symbolFile.open(QFile::ReadOnly)) {
            qWarning("Could not open %ls: %ls", qUtf16Printable(symbolFileName), qUtf16Printable(symbolFile.errorString()));
            return false;
        }

        m_zipWriter->addDirectory(symbolDir);
        m_zipWriter->addFile(symbolFileName, &symbolFile);

        return true;
    }

    QString moduleVersion(const QString &fileName)
    {
        QFile file{fileName};

        if (!file.open(QFile::ReadOnly)) {
            qWarning("Could not open %ls: %ls", qUtf16Printable(fileName), qUtf16Printable(file.errorString()));
            return {};
        }

        while (!file.atEnd()) {
            const auto line = file.readLine().split(' ');
            if (line.length() >= 4 && line[0] == "MODULE")
                return QString::fromUtf8(line[3]);
        }

        return {};
    }

    std::unique_ptr<QZipWriter> m_zipWriter;
    QStringList m_libraryPath;
    QString m_dumpsyms;
    QString m_objdump;
    QDir m_workDir;
};

} // namespace KDHockeyApp

int main(int argc, char *argv[])
{
    return KDHockeyApp::CollectSymbols{argc, argv}.run();
}
