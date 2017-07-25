/*
 *
 *
 */
#ifndef SPACEMAP_H_
#define SPACEMAP_H_


class TH1;

namespace Map {
  
  typedef TH1 SpaceMap;

}


namespace Map {

  enum Axis { kAxisX, kAxisY, kAxisZ };
  enum Ref { kLocal, kGlobal};
 
}


#endif



