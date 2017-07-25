/*
 * SpaceMapMgr.h
 *
 *
 *
 */

#ifndef SPACEMAPMGR_H_
#define SPACEMAPMGR_H_

//
#include <TString.h>
#include <TObjArray.h>


//
#include "SpaceMap.h"
#include "SpaceMap3.h"
#include "SpaceMapSet.h"
#include "SpaceMapFolder.h"



struct mystr {

  std::vector<float> v;
};

namespace Map {

  class SpaceMapMgr: public TObjArray {

    
  public:
    SpaceMapMgr();
    
    bool AddFolder(Map::SpaceMapFolder*);
    
    Map::SpaceMapFolder* GetFolder(const TString &) const;

    
  private:

    
  };


}




#endif
