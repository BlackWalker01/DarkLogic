/*===--- allproposition.h - include for DarkLogic library --------------*- C++ -*-===*
*
*   Part of the DarkLogic Project, under the CC0 license.
*   See https://creativecommons.org/publicdomain/zero/1.0/deed.fr for license information.
*
*===----------------------------------------------------------------------===*
*
*   This file includes all proposition files for DarkLogic Library
*   Author: BlackWalker
*   Date: april 12 2020
*
*===----------------------------------------------------------------------===*/

#ifndef DARK_LOGIC_ALLPROPOSITION_H
#define DARK_LOGIC_ALLPROPOSITION_H
#include "rule.h"
#include "theorem.h"

//Generic properties
#include "Generic/subrulegeneric.h"
#include "Generic/subtheoremgeneric.h"
#include "Generic/theoremgeneric.h"

//Hyp properties
#include "Hyp/subrulehyp.h"
#include "Hyp/subtheoremhyp.h"
#include "Hyp/theoremhyp.h"

//Not properties
#include "Not/subrulenot.h"
#include "Not/subtheoremnot.h"
#include "Not/theoremnot.h"

//Boolean properties
#include "Boolean/subruleboolean.h"
#include "Boolean/subtheoremboolean.h"
#include "Boolean/theoremboolean.h"

//ConstBoolean properties
#include "ConstBoolean/subruleconstboolean.h"
#include "ConstBoolean/subtheoremconstboolean.h"
#include "ConstBoolean/theoremconstboolean.h"

//Equal properties
#include "Equal/subruleequal.h"
#include "Equal/subtheoremequal.h"
#include "Equal/theoremequal.h"

//BelongsTo properties
#include "BelongsTo/subrulebelongsto.h"
#include "BelongsTo/subtheorembelongsto.h"
#include "BelongsTo/theorembelongsto.h"

//Let properties
#include "Let/subrulelet.h"
#include "Let/subtheoremlet.h"
#include "Let/theoremlet.h"

#endif // DARK_LOGIC_ALLPROPOSITION_H
