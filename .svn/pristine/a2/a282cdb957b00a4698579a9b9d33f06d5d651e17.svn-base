/*
 *
 *
 *
 *
 *
 */


#include "SpaceMapSet.h"


namespace Map {

  SpaceMapSet::SpaceMapSet() {
    /**
     *
     */

  }


  bool SpaceMapSet::AddMap(TString tag, Map::SpaceMap *map) {


    if( map == nullptr ) {
      return false;
    }
    
    Index[tag] = GetEntries();
    
    TObjArray::Add(map);
    
    // VG, added return value to this function
    return true;
  } 

  int SpaceMapSet::GetIndex(const TString& tag) const {

    //
    // std::map<TString,int>::iterator i = Index.find(id);
    // if(i==Index.end() ) rerturn nullptr;
    // int idx = i->second;
    //
    
    if( !Index.count(tag) ) return -1;

    return Index.find(tag)->second;
    
  }

  
  Map::SpaceMap* SpaceMapSet::GetMap(const TString& tag) const {
    
    int idx = GetIndex(tag);

    if( idx==-1) return nullptr;

    return GetMap(idx);
  }

  
  Map::SpaceMap* SpaceMapSet::GetMap(int idx) const {
    
    if( idx<0 || idx>GetEntries()-1 ) return nullptr;
    
    return (Map::SpaceMap*)At(idx);
  }

 

  
}/*namespace Map*/



