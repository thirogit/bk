#pragma once 

#include <SF/Archive.hpp>

#include "../FarmNo.h"
#include "../AccountNo.h"
#include "../HentNo.h"
#include "../../../types/DateTime.h"
#include "../../../types/NullDecimal.h"
#include "../../../types/Decimal.h"
#include "../CowSex.h"
#include "../CowNo.h"
#include "../../../types/Time.h"
#include "../../../types/NullInt.h"

void serialize(SF::Archive &archive,FarmNo& farmNo);
void serialize(SF::Archive &archive,AccountNo& accountNo);
void serialize(SF::Archive &archive,DateTime& dt);
void serialize(SF::Archive &archive,HentNo& hentNo);
void serialize(SF::Archive &archive,NullDecimal& decimal);
void serialize(SF::Archive &archive,Decimal& decimal);
void serialize(SF::Archive &archive,CowSex& sex);
void serialize(SF::Archive &archive,CowNo& cowNo);
void serialize(SF::Archive &archive,Time& time);
void serialize(SF::Archive &archive,NullInt& nullint);
