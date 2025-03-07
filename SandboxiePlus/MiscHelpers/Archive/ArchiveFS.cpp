#include "stdafx.h"
#include "ArchiveFS.h"
#include "../Common/Common.h"

//////////////////////////////////////////////////////////////////////////////////////////
// C7zFileEngineIterator
// 

class C7zFileEngineIterator : public QAbstractFileEngineIterator
{
public:
    C7zFileEngineIterator(QDir::Filters filters, const QStringList& filterNames, 
        const QStringList& allEntries)
        : QAbstractFileEngineIterator(filters, filterNames), entries(allEntries), index(0) {}
    ~C7zFileEngineIterator() {}

    QString next() override
    {
        if (!hasNext())
            return QString();
        ++index;
        return currentFilePath();
    }
    bool hasNext() const override
    {
        return index < entries.size();
    }

    QString currentFileName() const override
    {
        if (index <= 0 || index > entries.size())
            return QString();
        return entries.at(index - 1);
    }

private:
    mutable QStringList entries;
    mutable int index;
};


//////////////////////////////////////////////////////////////////////////////////////////
// C7zFileEngine
// 

C7zFileEngine::C7zFileEngine(const QString& filename, CArchive* pArchive, QMutex* pMutex)
    : _flags(0), _size(0), _pos(0), _pArchive(pArchive), _pMutex(pMutex)
{
    setFileName(filename);
}

C7zFileEngine::~C7zFileEngine()
{
    close();
}

bool C7zFileEngine::open(QIODevice::OpenMode openMode)
{
    close();

    //if (openMode & QIODevice::WriteOnly) {
    //    
    //} else 
    if (openMode & QIODevice::ReadOnly) {
        
        QMutexLocker Lock(_pMutex);

        QMap<int, QIODevice*> Files;
		Files.insert(_index, new QBuffer(&_data));
		_pArchive->Extract(&Files);

        _size = _data.size();
        _pos = 0;

    } else 
    //if (openMode & QIODevice::Append) {
    //    
    //} else 
    {
        qWarning("Bad file open mode: %d", (int)openMode);
        return false;
    }

    return true;
}

bool C7zFileEngine::close()
{
    if (!isOpened())
        return false;

    _data.clear();
    _size = 0;
    _pos = 0;

    return true;
}

bool C7zFileEngine::flush()
{
    return true;
}

qint64 C7zFileEngine::size() const
{
    return _size;
}

qint64 C7zFileEngine::pos() const
{
    return _pos;
}

bool C7zFileEngine::seek(qint64 pos)
{
    _pos = pos;
    return true;
}

bool C7zFileEngine::isSequential() const
{
    return true;
}

bool C7zFileEngine::remove()
{
    return false;
}

bool C7zFileEngine::mkdir(const QString& dirName, bool createParentDirectories) const
{
    return false;
}

bool C7zFileEngine::rmdir(const QString& dirName, bool recurseParentDirectories) const
{
    return false;
}

bool C7zFileEngine::caseSensitive() const
{
    return false;
}

bool C7zFileEngine::isRelativePath() const
{
    return false;
}

QAbstractFileEngine::Iterator *C7zFileEngine::beginEntryList(QDir::Filters filters, const QStringList &filterNames)
{
    QMutexLocker Lock(_pMutex);

    QStringList allEntries;
    for (int i = 0; i < _pArchive->FileCount(); i++) {
        QString Path = _pArchive->FileProperty(i, "Path").toString();
        if (!_filename.isEmpty()) {
            if (!Path.startsWith(_filename))
                continue;
            Path = Path.mid(_filename.length() + 1);
        }
        if (!Path.isEmpty() && !Path.contains("\\"))
            allEntries.append(Path);
    }

    return new C7zFileEngineIterator(filters, filterNames, allEntries);
}

QAbstractFileEngine::FileFlags C7zFileEngine::fileFlags(FileFlags type) const
{
    return type & _flags;
}

QString C7zFileEngine::fileName(FileName file) const
{
    return _filename;
}

QDateTime C7zFileEngine::fileTime(FileTime time) const
{
    switch (time)
    {
    case QAbstractFileEngine::ModificationTime:
    default:
        return _datetime;
        break;
    };
}

void C7zFileEngine::setFileName(const QString& file)
{
    int pos = file.indexOf(":") + 2;
    _filename = file.mid(pos);

    if (_filename.isEmpty()) { // root
        _flags = ExistsFlag | DirectoryType | ReadOwnerPerm | ReadUserPerm | ReadGroupPerm | ReadOtherPerm;
        return;
    }

    QMutexLocker Lock(_pMutex);
    _index = _pArchive->FindByPath(_filename);
    if (_index != -1) {
        _flags = ExistsFlag | ReadOwnerPerm | ReadUserPerm | ReadGroupPerm | ReadOtherPerm;
        if (_pArchive->FileProperty(_index, "IsDir").toBool())
            _flags |= DirectoryType;
        else
            _flags |= FileType;
    }
}

bool C7zFileEngine::atEnd() const
{
    return _pos >= _data.size();
}

qint64 C7zFileEngine::read(char* data, qint64 maxlen)
{
    qint64 togo = qMin(maxlen, _size - _pos);
    if (togo > 0) {
        memcpy(data, _data.constData() + _pos, togo);
        _pos += togo;
    }
    return togo;
}

qint64 C7zFileEngine::readLine(char* data, qint64 maxlen)
{
    return -1; // todo
}

qint64 C7zFileEngine::write(const char* data, qint64 len)
{
    return -1;
}

bool C7zFileEngine::isOpened() const
{
    return _size != 0;
}

QFile::FileError C7zFileEngine::error() const
{
    return QFile::UnspecifiedError;
}

QString C7zFileEngine::errorString() const
{
    return "";
}

bool C7zFileEngine::supportsExtension(Extension extension) const
{
    return extension == QAbstractFileEngine::AtEndExtension;
}



//////////////////////////////////////////////////////////////////////////////////////////
// C7zFileEngineHandler
// 

C7zFileEngineHandler::C7zFileEngineHandler(const QString& ArchivePath, const QString& Scheme, QObject* parent)
    : QObject(parent), m_pArchive(NULL)
{
    CArchive* pArchive = new CArchive(ArchivePath);
    if (pArchive->Open() > 0)
        m_pArchive = pArchive;
    else
        delete pArchive;
    m_Scheme = Scheme + ":";
}

C7zFileEngineHandler::~C7zFileEngineHandler()
{
    delete m_pArchive;
}

QAbstractFileEngine* C7zFileEngineHandler::create(const QString& filename) const
{
    if (m_pArchive && filename.startsWith(m_Scheme))
        return new C7zFileEngine(filename, m_pArchive, &m_Mutex);

    return NULL;
}