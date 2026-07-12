#pragma once
#include "pch.h"

class FString;
class TBitArray;
using FBitArray = TBitArray;
class FScriptArray;
class FScriptSparseArray;
class FScriptSet;
class FScriptMap;

template<typename ArrayElementType> class TArray;
template<typename ElementType> class TArrayView;
template<typename SetElementType> class TSet;
template<typename SetType> class TSetElement;
template<typename KeyElementType, typename ValueElementType> class TMap;
template<typename KeyElementType, typename ValueElementType> class TMultiMap;
template<typename KeyElementType, typename ValueElementType> class TSortedMap;
template<typename KeyType, typename ValueType> class TPair;
template<typename SparseArrayElementType> class TSparseArray;
template<typename InElementType> class TIndirectArray;
template<class TEnum> class TEnumAsByte;
