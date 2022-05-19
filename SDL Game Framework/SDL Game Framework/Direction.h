#pragma once

#include "Type.h"
#include "Framework/Csv.h"
#include "Framework/CsvParser.h"

void Direction_DataSetting(int32 DirectionNumber); // Scene 전환 시 필요한 연출에 맞춰 초기 데이터 세팅

void Direction_DataChange(int32 DirectionNumber); // Scene 연출 번호를 받아 필요한 계산식 실행