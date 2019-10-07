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
        args.addOption({"dumpsyms", "PATH", " The machine specific dumpsyms binary to use"});
        args.addOption({"readelf", "PATH", " The machine specific readelf binary to use"});
        args.addOption({"library-path", "PATH", " Directories of where to find shared libraries"});
        args.addPositionalArgument("TARGET", "The file from which to collect symbols");
        args.parse(arguments());

        const auto pargs = args.positionalArguments();

        if (pargs.size() != 2)
            return EXIT_FAILURE;

        m_dumpSyms = args.value("dumpsyms");
        m_readElf = args.value("readelf");

        if (m_dumpSyms.isEmpty())
            m_dumpSyms= "dump_syms";
        if (m_readElf.isEmpty())
            m_readElf = "readelf";

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

        if (!archive.endsWith(".zip"))
            return EXIT_FAILURE;

        m_workDir = archive.left(archive.length() - 4);
        m_workDir.removeRecursively();

        if (!m_workDir.mkpath(".")) {
            qWarning("Could not create work directory");
            return EXIT_FAILURE;
        }

        m_zipWriter = std::make_unique<QZipWriter>(archive);

        if (!dumpSymbols(target))
            return EXIT_FAILURE;

        const auto dependencyList = dependencies(target);
        if (!dependencyList.first)
            return EXIT_FAILURE;

        for (const auto &libraryName: dependencyList.second) {
            if (!dumpSymbols(findLibrary(libraryName)))
                return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

private:
    std::pair<bool, QStringList> dependencies(const QString &fileName)
    {
        QProcess readElf;
        readElf.setReadChannel(QProcess::StandardOutput);
        readElf.start(m_readElf, {"-d", fileName});

        if (!readElf.waitForReadyRead()) {
            qWarning("Could not run readelf: %ls", qUtf16Printable(readElf.errorString()));
            return {};
        }

        QRegularExpression pattern{".*\\(NEEDED\\).*\\[(.*)\\]"};
        QStringList dependencies;

        while (!readElf.atEnd()) {
            const auto line = QString::fromLocal8Bit(readElf.readLine());
            const auto test = pattern.match(line);

            if (test.hasMatch())
                dependencies.append(test.captured(1));
        }

        if (!readElf.waitForFinished())
            return {};

        return {true, dependencies};
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
        const QFileInfo symbolFileInfo{m_workDir.filePath(fileInfo.baseName() + ".sym")};

        QProcess dumpSyms;
        dumpSyms.setStandardOutputFile(symbolFileInfo.filePath());
        dumpSyms.start(m_dumpSyms, {"-v", fileInfo.filePath()});

        if (!dumpSyms.waitForFinished()) {
            qWarning("Could not run dumpsyms: %ls", qUtf16Printable(dumpSyms.errorString()));
            return false;
        }

        dumpSyms.close();

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
    QString m_dumpSyms;
    QString m_readElf;
    QDir m_workDir;
};

} // namespace KDHockeyApp

int main(int argc, char *argv[])
{
    return KDHockeyApp::CollectSymbols{argc, argv}.run();
}
