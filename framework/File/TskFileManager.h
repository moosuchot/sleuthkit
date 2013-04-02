/*
 * The Sleuth Kit
 *
 * Contact: Brian Carrier [carrier <at> sleuthkit [dot] org]
 * Copyright (c) 2010-2012 Basis Technology Corporation. All Rights
 * reserved.
 *
 * This software is distributed under the Common Public License 1.0
 */

/**
 * \file TskFileManager.h
 * Contains the interface for the TskFileManager class.
 */

#ifndef _TSK_FILEMANAGER_H
#define _TSK_FILEMANAGER_H

#include "framework_i.h"
#include "TskFile.h"

/**
 * Responsible for managing TskFile objects in the system.
 */
class TSK_FRAMEWORK_API TskFileManager
{
public:
    typedef std::unique_ptr<TskFile> FilePtr;
    typedef std::vector< FilePtr >   FilePtrList;
    
    virtual FilePtrList findFilesByName(const std::string& name) = 0;
    virtual FilePtrList findFilesByPattern(const std::string& namePattern, const std::string& pathPattern) = 0;
    virtual FilePtrList findFilesByExtension(const std::vector<std::string>& extensions) = 0;
    virtual FilePtrList findFilesByParent(const uint64_t parentFileId) = 0;
    virtual FilePtrList findFilesByFsFileType(TSK_FS_NAME_TYPE_ENUM fsFileType) = 0;
    virtual FilePtrList getFiles(const std::vector<uint64_t>& fileIds) = 0;

    /**
     * Return a TskFile object for a given file ID.
     * @param fileId ID of file to return object of.
     * @returns Pointer to file object. Caller must free it.
     * @throws TskException in case of error.
     */
    virtual TskFile * getFile(const uint64_t fileId) = 0;

    /** 
     * Return the fully qualified path to where the local instance of the file with the given ID
     * should exist.  This does not validate that the ID is for a file and does
     * not validate that the file actually exists. 
     * @param fileId Id of the file.
     * @returns Path to where local file should exist. 
     */
    virtual std::wstring getPath(const uint64_t fileId) = 0;

    /**
     * Save the file to the default location. 
     * @param fileToSave File object of the file to save.
     * @throws various exceptions on errors
     */
    virtual void saveFile(TskFile* fileToSave) = 0;

    /**
     * Save the file to the default location. 
     * @param fileId ID of the file to save.
     * @throws various exceptions on errors
     */
    virtual void saveFile(const uint64_t fileId)
    {
        saveFile(getFile(fileId));
    }

    /**
     * Copy the file to the given fully qualifed file path. 
     * Directories along the path will be created if they do not exist.
     * If the destination file exists it will be replaced.
     * @param fileToSave The file to save.
     * @param filePath The path to save to, including the file name. 
     * @throws various exceptions on errors
     */
    virtual void copyFile(TskFile* fileToSave, const std::wstring& filePath) = 0;

    /**
     * Copy the file to the given fully qualifed file path. 
     * Directories along the path will be created if they do not exist.
     * If the destination file exists it will be replaced.
     * @param fileId ID of the file to save.
     * @param filePath The path to save to, including the file name. 
     * @throws various exceptions on errors
     */
    virtual void copyFile(const uint64_t fileId, const std::wstring& filePath)
    {
        copyFile(getFile(fileId), filePath);
    }

    /**
     * Copy the contents of a directory to the given fully qualifed file path. 
     * Directories along the path will be created if they do not exist.
     * If the destination directory exists it will be replaced.
	 * Defaults to a non-recursive copy.
     * @param directoryToCopy The TskFile object representing the directory to copy.
     * @param destinationPath The path to save directory contents to, including the directory name.
	 * @param bRecurse Whether to recursively copy directory contents.
     * @throws various exceptions on errors
     */
    virtual void copyDirectory(TskFile* directoryToCopy, const std::wstring& destinationPath, const bool bRecurse = false) = 0;

    /**
     * Copy the contents of a directory to the given fully qualifed file path. 
     * Directories along the path will be created if they do not exist.
     * If the destination directory exists it will be replaced.
	 * Defaults to a non-recursive copy.
     * @param directoryIdToCopy The id representing the directory to copy.
     * @param destinationPath The path to save directory contents to, including the directory name. 
	 * @param bRecurse Whether to recursively copy directory contents.
     * @throws various exceptions on errors
     */
    virtual void copyDirectory(uint64_t directoryIdToCopy, const std::wstring& destinationPath, const bool bRecurse = false)
    {
        copyDirectory(getFile(directoryIdToCopy), destinationPath, bRecurse);
    }

    /**
     * Add a file to the system using the given file id and input stream.
     * This method saves a local copy of the content contained in the input stream.
     * @param fileId ID of the new file.
     * @param istr Input stream containing the file content to save.
     * @throws TskFileException if a file with the given fileId already exists or
     * if an error is encountered while saving the input stream.
     */
    virtual void addFile(const uint64_t fileId, std::istream& istr) = 0;

    /**
     * Add a file to the system using the given file id and path.
     * This method saves a local copy of the file given in the path.
     * @param fileId ID of the new file.
     * @param filePath The path of the file to save.
     * @throws TskFileException if a file with the given fileId already exists,
     * the file specified in filePath does not exist or an error is encountered 
     * while saving the file.
     */
    virtual void addFile(const uint64_t fileId, std::wstring& filePath) = 0;

    /**
     * Delete the local copy of a file.
     * @param fileToDelete Object of file to delete local copy of
     * @throws various exceptions on errors
     */
    virtual void deleteFile(TskFile* fileToDelete) = 0;

    /**
     * Delete the local copy of a file.
     * @param fileId ID of file to delete local copy of
     * @throws various exceptions on errors
     */
    virtual void deleteFile(const uint64_t fileId)
    {
        deleteFile(getFile(fileId));
    }

protected:
    /// Default Constructor
    TskFileManager() {};

    /// Copy Constructor
    TskFileManager(TskFileManager const&) {};

    /// Destructor
    virtual ~TskFileManager() {};
};

#endif
