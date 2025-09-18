#pragma once
#include <iostream>
#include "TError.h"

typedef unsigned int TELEM;

class TBitFields {
private:
	int bitLen;
	TELEM* pMem;
	int memLen;

	int GetMemIndex(const int n) const;
	TELEM GetMemMask(const int n) const;
public:
	TBitFields();
	TBitFields(int len);
	TBitFields(const TBitFields& bf);
	TBitFields(TBitFields&& bf) noexcept;
	~TBitFields();

	int GetLength() const;
	void SetBit(const int n);
	void ClrBit(const int n);
	int GetBit(const int n) const;

	bool operator==(const TBitFields& bf) const;
	bool operator!=(const TBitFields& bf) const;
	TBitFields& operator=(const TBitFields& bf);
	TBitFields& operator=(TBitFields&& bf) noexcept;
	TBitFields operator|(const TBitFields& bf);
	TBitFields operator&(const TBitFields& bf);
	TBitFields operator~();

	friend std::istream& operator>> (std::istream& in, TBitFields&bf);
	friend std::ostream& operator<< (std::ostream& out, TBitFields& bf);
	friend std::ostream& operator<<(std::ostream& out, const TBitFields& bf);
};