/* ====================================================================================================================

  The copyright in this software is being made available under the License included below.
  This software may be subject to other third party and contributor rights, including patent rights, and no such
  rights are granted under this license.

  Copyright (c) 2012, SAMSUNG ELECTRONICS CO., LTD. All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, are permitted only for
  the purpose of developing standards within Audio and Video Coding Standard Workgroup of China (AVS) and for testing and
  promoting such standards. The following conditions are required to be met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
      the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
      the following disclaimer in the documentation and/or other materials provided with the distribution.
    * The name of SAMSUNG ELECTRONICS CO., LTD. may not be used to endorse or promote products derived from 
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 * ====================================================================================================================
*/

/** \file     TEncGOP.h
    \brief    GOP encoder class (header)
*/

#ifndef __TENCGOP__
#define __TENCGOP__

#include <stdlib.h>

#include "../TLibCommon/TComList.h"
#include "../TLibCommon/TComPic.h"
#include "../TLibCommon/TComBitStream.h"
#include "../TLibCommon/TComBitCounter.h"
#include "TEncLoopFilter.h"
#include "TEncPicture.h"
#include "TEncEntropy.h"
#include "TEncSbac.h"

#include "TEncAnalyze.h"

class TEncTop;

// ====================================================================================================================
// Class definition
// ====================================================================================================================

/// GOP encoder class
class TEncGOP
{
private:
  //  Data
  Int                     m_iHrchDepth;
  Int                     m_iGopSize;
  Int                     m_iRateGopSize;
  Int                     m_iNumPicCoded;
  Bool                    m_bFirst;
  
  //  Access channel
  TEncTop*                m_pcEncTop;
  TEncCfg*                m_pcCfg;
  TEncPicture*              m_pcPictureEncoder;
  TComList<TComPic*>*     m_pcListPic;
  
  TEncEntropy*            m_pcEntropyCoder;
  TEncSbac*               m_pcSbacCoder;
  TEncLoopFilter*         m_pcLoopFilter;
  
  TComBitCounter*         m_pcBitCounter;
  
  // indicate sequence first
  Bool                    m_bSeqFirst;
#if B_RPS_BUG
  Bool                    m_bSeqEnd;
  UInt                    m_uiSeqEndGOPPicNum;
#endif

public:
  TEncGOP();
  virtual ~TEncGOP();
  
  Void  create      ();
  Void  destroy     ();
  
  Void  init        ( TEncTop* pcTEncTop );
  Void  compressGOP ( Int iPOCLast, Int iNumPicRcvd, TComList<TComPic*>& rcListPic, TComList<TComPicYuv*>& rcListPicYuvRec, TComList<TComBitstream*> rcListBitstream );
  
  Int   getGOPSize()          { return  m_iGopSize;  }
  Int   getRateGOPSize()      { return  m_iRateGopSize;  }
  Int   isHierarchicalB()     { return  m_pcCfg->getHierarchicalCoding();  }
  Int   getHrchDepth()        { return  m_iHrchDepth; }
  
  TComList<TComPic*>*   getListPic()      { return m_pcListPic; }
#if wlq_WQ
  Void InitFrameQuantParam(WeightQuantSyntax* WeightQuantSyntax);
  Void FrameUpdateWQMatrix(WeightQuantSyntax* WeightQuantSyntax);        //M2148 2007-09
  Void CalculateQuantParam(UChar uiWQMSizeId);
#endif
#if wlq_FME
  Void InitRdCostMvBits();
#endif
  Void  printOutSummary      ( UInt uiNumAllPicCoded );
  Void  preLoopFilterPicAll  ( TComPic* pcPic, UInt64& ruiDist, UInt64& ruiBits );
  
  TEncPicture*  getPictureEncoder()   { return m_pcPictureEncoder; }
#if B_RPS_BUG
  Bool getSeqEnd() { return m_bSeqEnd; }
  UInt getSeqEndGOPPicNum() { return m_uiSeqEndGOPPicNum; }
#endif
protected:
  Void  xInitGOP          ( Int iPOC, Int iNumPicRcvd, TComList<TComPic*>& rcListPic, TComList<TComPicYuv*>& rcListPicYuvRecOut );
  Void  xGetBuffer        ( TComList<TComPic*>& rcListPic, TComList<TComPicYuv*>& rcListPicYuvRecOut, TComList<TComBitstream*>& rcListBitstream, Int iNumPicRcvd, Int iTimeOffset, TComPic*& rpcPic, TComPicYuv*& rpcPicYuvRecOut, TComBitstream*& rpcBitstreamOut, UInt uiPOCCurr );
  
  Void  xCalculateAddPSNR ( TComPic* pcPic, TComPicYuv* pcPicD, UInt uiBits );
  Void  xCalculateAddPSNR ( TComPic* pcPic, TComPicYuv* pcPicD, UInt uiBits, Double dEncTime );
  
  UInt64 xFindDistortionFrame (TComPicYuv* pcPic0, TComPicYuv* pcPic1);
};// END CLASS DEFINITION TEncGOP


#endif // __TENCGOP__

