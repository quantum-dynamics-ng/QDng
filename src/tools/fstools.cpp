#include "fstools.h"
#include "sys/stat.h"
#include "sys/errno.h"


namespace QDLIB {
   
   /**
    * Strip the last component from path.
    * 
    * Doesn't matter if it is a file or directory.
    */
   void StripLast(string& dir)
   {
      size_t pos;
      
      /* strip trailing slashes */
      while (dir[dir.length()-1] == '/')
         dir.erase(dir.length()-1,1);
      cout << dir << endl;
      /* Erase last dir in string */
      pos = dir.find_last_of("/");
      if (pos != dir.npos)
         dir.erase(pos);
      else
         dir.erase();
      
      cout << dir << endl;
   }
   
   void CreateDir(const string & dir)
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
         
         
         cout << "ENOENT" << ENOENT << endl;
         cout << errno << " " << ret << endl;
         
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

   
}




