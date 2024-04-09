// SPDX-FileCopyrightText: 2024 Markus Kowalewski, Regina de Vivie-Riedle
//
// SPDX-License-Identifier: gpl-3-only

#include "fstools.h"
#include "sys/stat.h"
#include "sys/errno.h"
#include <stdio.h>


namespace QDLIB {
   
   /**
    * Strip the last component from path.
    * 
    * Doesn't matter if it is a file or directory.
    */
   void FS::StripLast(string& dir)
   {
      size_t pos;
      
      /* strip trailing slashes */
      while (dir[dir.length()-1] == '/')
         dir.erase(dir.length()-1,1);
      
      /* Erase last dir in string */
      pos = dir.find_last_of("/");
      if (pos != dir.npos)
         dir.erase(pos);
      else
         dir.erase();
   }
   
   /** 
    * Create a directory.
    * 
    * If the directory doesn't exist nothing happens.
    * If dir is path with more than one non-existent directories, then
    * the full path will be created.
    * 
    */
   void FS::CreateDir(const string & dir)
   {
      struct stat statbuf;
      int ret;

      /* Nothing to do */
      if (dir.empty() == true || dir == "./") return;
      
      /* Check if dir already exists */
      ret =  stat(dir.c_str(), &statbuf);
      
      /* Create dir */
      if (ret != 0 && errno == ENOENT){
         /* Try to create upper levels levels */
         string s(dir);
         StripLast(s);
         cout << s;
         CreateDir(s);

         mode_t cmode;
         cmode = S_IRWXU | S_IRWXG | S_IRWXO;
         ret = mkdir (dir.c_str(), cmode);
         
         if (ret != 0)
            throw ( EIOError(errno, dir) );
         
         return;
      }
      
      /* something exists - check if it is correct dir */
      if (ret == 0){
         if (S_ISDIR(statbuf.st_mode)){
            return;
         } else {
            throw ( EIOError ("Not a directory", dir) );
         }
      }
      
      /* Something went wrong */
      if (ret != 0)
         throw ( EIOError(errno, dir) );
   }

   /** Remove file or empty directory. */
   void FS::Remove(const string& name)
   {
      if ( remove(name.c_str()) != 0 ){
         switch (errno) {
            case EACCES:
                  throw ( EIOError ("Can't remove file. Access denied: ", name) );
               break;
            case ENOENT:
                  throw ( EIOError ("Can't remove file. File does not exist: ", name) );
               break;
            default:
               throw ( EIOError ("Can't remove file. Error: ", name) );
         }
      }
   }
   
   
   bool FS::IsDir(const string & name)
   {
      struct stat statbuf;
      int ret;
      
      ret =  stat(name.c_str(), &statbuf);
      
      if (ret != 0)
	 throw ( EIOError(errno, name) );
      
      if (S_ISDIR(statbuf.st_mode)) return true;
      
      return false;
   }

   bool FS::IsFile(const string & name)
   {
      struct stat statbuf;
      int ret;
      
      ret =  stat(name.c_str(), &statbuf);
      
      if (ret != 0 && errno != ENOENT)
         throw ( EIOError(errno, name) );
      else if (ret != 0 && errno == ENOENT)
         return false;
      
      if (S_ISREG(statbuf.st_mode)) return true;
      
      return false;
   }
   
   
} /* namespace QDLIB */






