/**
 * @file VoltToDac.cpp
 * @brief Source file for class VoltToDac
 * @date 06 mag 2019
 * @author pc
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class VoltToDac (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "VoltToDacGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

VoltToDacGAM::VoltToDacGAM() :
        GAM() {
    maxIn = 0;
    minIn = 0;
    maxOut = 0;
    minOut = 0;

    voltage = NULL;
    output = NULL;
    numberOfElements=NULL;
}

VoltToDacGAM::~VoltToDacGAM() {
    voltage = NULL;
    output = NULL;
    if(numberOfElements!=NULL){
        delete [] numberOfElements;
    }
}

bool VoltToDacGAM::Initialise(MARTe::StructuredDataI & data) {
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.Read("MaxIn", maxIn);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Please define MaxIn");
        }
    }
    if (ok) {
        ok = data.Read("MinIn", minIn);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Please define MinIn");
        }
    }
    if (ok) {
        ok = data.Read("MinOut", minOut);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Please define MinOut");
        }
    }
    if (ok) {
        ok = data.Read("MaxOut", maxOut);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Please define MaxOut");
        }
    }
    return ok;
}

bool VoltToDacGAM::Setup() {
    numberOfElements= new uint32[numberOfInputSignals];
    for (uint32 i = 0u; i < numberOfInputSignals; i++) {
        GetSignalNumberOfElements(InputSignals, i, numberOfElements[i]);
    }
    voltage = reinterpret_cast<float64 *>(GetInputSignalMemory(0));
    output = reinterpret_cast<uint16 *>(GetOutputSignalMemory(0));
    return true;
}

bool VoltToDacGAM::Execute() {
    uint32 cnt = 0u;
    for (uint32 i = 0u; i < numberOfInputSignals; i++) {
        for (uint32 j = 0u; j < numberOfElements[i]; j++) {
            float64 factor = (voltage[cnt] - minIn) / ((maxIn - minIn));
            output[cnt] = (uint16)(minOut + (factor * (maxOut - minOut)));
            cnt++;
        }
    }
    return true;
}
CLASS_REGISTER(VoltToDacGAM, "")

}
