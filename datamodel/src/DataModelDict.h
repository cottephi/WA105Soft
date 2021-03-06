/********************************************************************
* src/DataModelDict.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error src/DataModelDict.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#define G__PRIVATE_GVALUE
#include "G__ci.h"
#include "FastAllocString.h"
extern "C" {
extern void G__cpp_setup_tagtableDataModelDict();
extern void G__cpp_setup_inheritanceDataModelDict();
extern void G__cpp_setup_typetableDataModelDict();
extern void G__cpp_setup_memvarDataModelDict();
extern void G__cpp_setup_globalDataModelDict();
extern void G__cpp_setup_memfuncDataModelDict();
extern void G__cpp_setup_funcDataModelDict();
extern void G__set_cpp_environmentDataModelDict();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "RunHeader.h"
#include "EventHeader.h"
#include "BeamInfo.h"
#include "GeomConfig.h"
#include "DataFragment.h"
#include "DataChan.h"
#include "DataCRM.h"
#include "DataCRP.h"
#include "VoxelInfo.h"
#include "VoxelInfoCRM.h"
#include "VoxelInfoCRP.h"
#include "RecoObject.h"
#include "QHit.h"
#include "Cluster2d.h"
#include "Track2d.h"
#include "RecoConfig.h"
#include "HitReco.h"
#include "DataLRO.h"
#include "ChanTrueQ.h"
#include "AnodeTrueQ.h"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__DataModelDictLN_TClass;
extern G__linked_taginfo G__DataModelDictLN_TBuffer;
extern G__linked_taginfo G__DataModelDictLN_TMemberInspector;
extern G__linked_taginfo G__DataModelDictLN_TObject;
extern G__linked_taginfo G__DataModelDictLN_TString;
extern G__linked_taginfo G__DataModelDictLN_vectorlEunsignedsPshortcOallocatorlEunsignedsPshortgRsPgR;
extern G__linked_taginfo G__DataModelDictLN_vectorlEfloatcOallocatorlEfloatgRsPgR;
extern G__linked_taginfo G__DataModelDictLN_vectorlEdoublecOallocatorlEdoublegRsPgR;
extern G__linked_taginfo G__DataModelDictLN_string;
extern G__linked_taginfo G__DataModelDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__DataModelDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__DataModelDictLN_TList;
extern G__linked_taginfo G__DataModelDictLN_TObjArray;
extern G__linked_taginfo G__DataModelDictLN_TClonesArray;
extern G__linked_taginfo G__DataModelDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR;
extern G__linked_taginfo G__DataModelDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__DataModelDictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR;
extern G__linked_taginfo G__DataModelDictLN_RunHeader;
extern G__linked_taginfo G__DataModelDictLN_timespec;
extern G__linked_taginfo G__DataModelDictLN_TTimeStamp;
extern G__linked_taginfo G__DataModelDictLN_TVector2;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTBaselEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTBaselEdoublegR;
extern G__linked_taginfo G__DataModelDictLN_TVectorTlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TVectorTlEdoublegR;
extern G__linked_taginfo G__DataModelDictLN_TElementActionTlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TElementPosActionTlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTRow_constlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTRowlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTDiag_constlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTColumn_constlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTFlat_constlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTSub_constlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTSparseRow_constlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTSparseDiag_constlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTColumnlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTDiaglEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTFlatlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTSublEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTSparseRowlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TMatrixTSparseDiaglEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_TParticle;
extern G__linked_taginfo G__DataModelDictLN_EventHeader;
extern G__linked_taginfo G__DataModelDictLN_BeamInfo;
extern G__linked_taginfo G__DataModelDictLN_GeomConfig;
extern G__linked_taginfo G__DataModelDictLN_LArRawData;
extern G__linked_taginfo G__DataModelDictLN_LArRawDatacLcLCode;
extern G__linked_taginfo G__DataModelDictLN_LArRawDatacLcLDtype;
extern G__linked_taginfo G__DataModelDictLN_LArRawDatacLcLDataChan;
extern G__linked_taginfo G__DataModelDictLN_LArRawDatacLcLDataCRM;
extern G__linked_taginfo G__DataModelDictLN_LArRawDatacLcLDataCRP;
extern G__linked_taginfo G__DataModelDictLN_lesslEstringgR;
extern G__linked_taginfo G__DataModelDictLN_LArVoxel;
extern G__linked_taginfo G__DataModelDictLN_LArVoxelcLcLVoxelInfo;
extern G__linked_taginfo G__DataModelDictLN_LArVoxelcLcLVoxelInfoCRM;
extern G__linked_taginfo G__DataModelDictLN_LArVoxelcLcLVoxelInfoCRP;
extern G__linked_taginfo G__DataModelDictLN_View_t;
extern G__linked_taginfo G__DataModelDictLN_LArReco;
extern G__linked_taginfo G__DataModelDictLN_LArRecocLcLRecoObj_t;
extern G__linked_taginfo G__DataModelDictLN_LArRecocLcLRecoObjLink;
extern G__linked_taginfo G__DataModelDictLN_LArRecocLcLRecoObject;
extern G__linked_taginfo G__DataModelDictLN_vectorlELArRecocLcLRecoObjLinkcOallocatorlELArRecocLcLRecoObjLinkgRsPgR;
extern G__linked_taginfo G__DataModelDictLN_vectorlELArRecocLcLRecoObjLinkcOallocatorlELArRecocLcLRecoObjLinkgRsPgRcLcLiterator;
extern G__linked_taginfo G__DataModelDictLN_reverse_iteratorlEvectorlELArRecocLcLRecoObjLinkcOallocatorlELArRecocLcLRecoObjLinkgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__DataModelDictLN_LArRecocLcLQHit;
extern G__linked_taginfo G__DataModelDictLN_LArRecocLcLCluster2d;
extern G__linked_taginfo G__DataModelDictLN_vectorlEconstsPLArRecocLcLQHitmUcOallocatorlEconstsPLArRecocLcLQHitmUgRsPgR;
extern G__linked_taginfo G__DataModelDictLN_reverse_iteratorlEvectorlEconstsPLArRecocLcLQHitmUcOallocatorlEconstsPLArRecocLcLQHitmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__DataModelDictLN_LArRecocLcLTrack2d;
extern G__linked_taginfo G__DataModelDictLN_vectorlETVector2cOallocatorlETVector2gRsPgR;
extern G__linked_taginfo G__DataModelDictLN_vectorlETVector2cOallocatorlETVector2gRsPgRcLcLiterator;
extern G__linked_taginfo G__DataModelDictLN_reverse_iteratorlEvectorlETVector2cOallocatorlETVector2gRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__DataModelDictLN_RecoConfig;
extern G__linked_taginfo G__DataModelDictLN_allocatorlEpairlEconstsPstringcOvectorlEfloatcOallocatorlEfloatgRsPgRsPgRsPgR;
extern G__linked_taginfo G__DataModelDictLN_maplEstringcOvectorlEfloatcOallocatorlEfloatgRsPgRcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOvectorlEfloatcOallocatorlEfloatgRsPgRsPgRsPgRsPgR;
extern G__linked_taginfo G__DataModelDictLN_pairlEstringcOvectorlEfloatcOallocatorlEfloatgRsPgRsPgR;
extern G__linked_taginfo G__DataModelDictLN_maplEstringcOvectorlEfloatcOallocatorlEfloatgRsPgRcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOvectorlEfloatcOallocatorlEfloatgRsPgRsPgRsPgRsPgRcLcLiterator;
extern G__linked_taginfo G__DataModelDictLN_maplEstringcOvectorlEfloatcOallocatorlEfloatgRsPgRcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOvectorlEfloatcOallocatorlEfloatgRsPgRsPgRsPgRsPgRcLcLreverse_iterator;
extern G__linked_taginfo G__DataModelDictLN_pairlEmaplEstringcOvectorlEfloatcOallocatorlEfloatgRsPgRcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOvectorlEfloatcOallocatorlEfloatgRsPgRsPgRsPgRsPgRcLcLiteratorcOboolgR;
extern G__linked_taginfo G__DataModelDictLN_HitReco;
extern G__linked_taginfo G__DataModelDictLN_LArRawDatacLcLDataLRO;
extern G__linked_taginfo G__DataModelDictLN_LArMCTruth;
extern G__linked_taginfo G__DataModelDictLN_LArMCTruthcLcLChanTrueQ;
extern G__linked_taginfo G__DataModelDictLN_LArMCTruthcLcLAnodeTrueQ;
extern G__linked_taginfo G__DataModelDictLN_vectorlELArMCTruthcLcLChanTrueQmUcOallocatorlELArMCTruthcLcLChanTrueQmUgRsPgR;
extern G__linked_taginfo G__DataModelDictLN_reverse_iteratorlEvectorlELArMCTruthcLcLChanTrueQmUcOallocatorlELArMCTruthcLcLChanTrueQmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__DataModelDictLN_LArRawDatacLcLDataFragmentlEfloatgR;
extern G__linked_taginfo G__DataModelDictLN_LArRawDatacLcLDataFragmentlEunsignedsPshortgR;

/* STUB derived class for protected member access */
typedef vector<LArReco::RecoObjLink,allocator<LArReco::RecoObjLink> > G__vectorlELArRecocLcLRecoObjLinkcOallocatorlELArRecocLcLRecoObjLinkgRsPgR;
typedef vector<TVector2,allocator<TVector2> > G__vectorlETVector2cOallocatorlETVector2gRsPgR;
typedef map<string,vector<float,allocator<float> >,less<string>,allocator<pair<const string,vector<float,allocator<float> > > > > G__maplEstringcOvectorlEfloatcOallocatorlEfloatgRsPgRcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOvectorlEfloatcOallocatorlEfloatgRsPgRsPgRsPgRsPgR;
typedef LArRawData::DataFragment<float> G__LArRawDatacLcLDataFragmentlEfloatgR;
typedef LArRawData::DataFragment<unsigned short> G__LArRawDatacLcLDataFragmentlEunsignedsPshortgR;
