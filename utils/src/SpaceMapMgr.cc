/*
 *
 *
 *
 *
 */

//
#include <cstddef>
#include <iostream>

//
#include <TObject.h>


//
#include "SpaceMapFolder.h"
#include "SpaceMapMgr.h"


namespace Map {

  SpaceMapMgr::SpaceMapMgr() {

  }

  bool SpaceMapMgr::AddFolder(Map::SpaceMapFolder *folder) {

    if( folder==nullptr) {
      return false;
    }

    TObjArray::Add(folder);
    
    return true;
  }

  Map::SpaceMapFolder* SpaceMapMgr::GetFolder(const TString& name) const {

   
    for(int i=0; i<GetEntries(); ++i) {

      TObject *o = At(i);

      if( o->GetName() == name ) {
	  return static_cast<Map::SpaceMapFolder*>(o);
      }
      
    }

    return nullptr;
  }


  
}

