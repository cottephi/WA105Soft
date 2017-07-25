/*
 * SpaceMapSet.h
 *
 *  Created on:
 *      Author:
 *
 *
 */

#ifndef SPACEMAPSET_H_
#define SPACEMAPSET_H_

//
#include <map>


//
#include <TObjArray.h>


//
#include "SpaceMap3.h" 


namespace Map {

  class SpaceMapSet: public TObjArray {
    
  public:
    SpaceMapSet();
    bool AddMap(TString tag, Map::SpaceMap *);

    int GetIndex(const TString& tag) const;
    
    Map::SpaceMap* GetMap(const TString& tag) const;
    Map::SpaceMap* GetMap(int) const;
    
      
  private:
    std::map<TString,int> Index;
    
  };

}/* namespace Map */
#endif /* SPACEMAPSET_H_ */
