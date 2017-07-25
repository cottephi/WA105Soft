/*
 *
 *
 *
 *
 *
 */

//
#include <iostream>
#include <vector>

//
#include <TObject.h>

//
#include "StringUtils.h"

//
#include "SpaceMapSet.h"
#include "SpaceMapFolder.h"




//
using std::cout;
using std::endl;

namespace Map {


  SpaceMapFolder::SpaceMapFolder() {
    /**
     *
     */

  }



  bool SpaceMapFolder::AddSet(Map::SpaceMapSet *set, int id) {
    /*
     *
     */

    if( set == nullptr) {
       cout<<" ########### "<<__FUNCTION__<<" null "<<endl;
      return false;
    }

    //cout<<" Adding "<<set<<" "<<id<<endl;

    Index[id] = GetEntries();

    //    cout<<" Adding "<<set<<" "<<id<<" index "<<Index[id]<<endl;

    TObjArray::Add(set);

    // VG, added return value to this function
    return true;
  }




  Map::SpaceMapSet* SpaceMapFolder::GetSet(int id) const {

    //
    // std::map<int,int>::iterator i = Index.find(id);
    // if(i==Index.end() ) rerturn nullptr;
    // int idx = i->second;
    //

    if( !Index.count(id) ) {

      std::cout<<__PRETTY_FUNCTION__<<" Set "<<id<<" not found "<<endl;
      return nullptr;
    }

    int idx = Index.find(id)->second;

    return (Map::SpaceMapSet*)At(idx);

  }




  void SpaceMapFolder::ImportSet(TFile &file, TString maps, TString format) {

    /**
     *
     *
     */

    cout<<" -- SpaceMapFolder::"<<__FUNCTION__<<" -- "<<endl;


    if(  file.IsZombie() ) {
        cout<<__FILE__<<" "<<__FUNCTION__<<" invalid input file "<<endl;
        return ;
    }

    std::vector<TString> map_list = StringUtils::Tokenize(maps,":");

    std::vector<TString> obj_list = StringUtils::Tokenize(format,":");

    for(int i=0; i<(int)map_list.size(); ++i) {

      cout<<" "<<i<<" "<<map_list[i]<<" "<<obj_list[i]<<endl;

    }


    Map::SpaceMapSet *mapSet = new Map::SpaceMapSet();


    for( int iob = 0; iob< (int)obj_list.size(); ++iob ) {

        TString obj_name = obj_list[iob];

        //TH3F *h3 = (TH3F*)(file.Get(obj_name));
        //TObject *obj = (TObject*)(file.Get(obj_name));

        TH3F *h3 = static_cast<TH3F*>(file.Get(obj_name));

        if(h3 == nullptr) {
            cout<<" ########### "<<obj_name<<" not found "<<h3<<endl;
            cout<<" file content "<<endl;
            cout<<" ---- "<<endl;
            continue;
        }

         Map::SpaceMap3 *map = new SpaceMap3();

	 map->Import(*h3);

	 mapSet->AddMap(map_list[iob],map);

        //cout<<" "<<h3<<endl;
        //cout<<" "<<map_list[iob]<<" "<<obj_name<<" "<<h3->GetName()<<endl;

      }//iob


        AddSet(mapSet,GetEntries());

  }




  void SpaceMapFolder::ImportSet(TFile &file, TString maps, TString format, int ifirst, int ilast) {

    /**
     *
     *
     */

    std::vector<TString> map_list = StringUtils::Tokenize(maps,":");

    std::vector<TString> obj_list = StringUtils::Tokenize(format,":");

    for(int i=0; i<(int)map_list.size(); ++i) {

      cout<<" "<<i<<" "<<map_list[i]<<" "<<obj_list[i]<<endl;

    }

    for(int i=ifirst; i<=ilast;++i) { // loop on pmts


      Map::SpaceMapSet *mapSet = new Map::SpaceMapSet();


      for( int iob = 0; iob< (int)obj_list.size(); ++iob ) { //3 tags

        TString obj_name = obj_list[iob];

        TString tag_id = ""; tag_id+=i;

        obj_name.ReplaceAll("@",tag_id);


        TH3F *h3 = static_cast<TH3F*>(file.Get(obj_name));


	h3->SetDirectory(0);

        if(h3 == nullptr) {
            cout<<" ########### "<<obj_name<<" not found "<<endl;
            continue;
        }

        Map::SpaceMap3 *map = new SpaceMap3();

        map->Import(*h3);

        mapSet->AddMap(map_list[iob],map);

	//        cout<<" "<<h3<<endl;
	//        cout<<" "<<map_list[iob]<<" "<<obj_name<<" "<<h3->GetName()<<endl;

      }//


      AddSet(mapSet,i);

    }



  }



}/*namespace Map*/



