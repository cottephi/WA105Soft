/*
 * SpaceMapSet.h
 *
 *  Created on:
 *      Author:
 *
 *
 */

#ifndef SPACEMAPFLD_H_
#define SPACEMAPFLD_H_

//
#include <map>


//
#include <TObjArray.h>
#include <TFile.h>


//
#include <StringUtils.h>

//
#include "SpaceMap.h"
#include "SpaceMap3.h"
#include "SpaceMapSet.h"


namespace Map {

  class SpaceMapFolder: public TObjArray {

  public:
    SpaceMapFolder();


    void ImportSet(TFile &, TString maps, TString format, int ifirst, int ilast);
    void ImportSet(TFile &, TString maps, TString format);


    bool AddSet(Map::SpaceMapSet *, int);
    int GetNumOfSet() const { return GetEntries(); }

    Map::SpaceMapSet* GetSet(int) const;

  private:
    std::map<int, int> Index;

  };

}/* namespace Map */
#endif /* SPACEMAPFLD_H_ */
