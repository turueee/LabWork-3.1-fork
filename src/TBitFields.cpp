#include "TBitFields.h"


#define ERROR(err,...) throw TError(err, __func__, __FILE__, __LINE__)
constexpr int BITSIZE = 8 * sizeof(TELEM);

int TBitFields::GetMemIndex(const int n) const
{
	if (n<0 || n>=bitLen)
		ERROR("Error_size");
	return n / BITSIZE;
}

TELEM TBitFields::GetMemMask(const int n) const
{
	if (n < 0 || n >= bitLen)
		ERROR("Error_size");
	return static_cast<TELEM>(1) << (n % BITSIZE);
}

TBitFields::TBitFields()
{
	bitLen = 0;
	pMem = nullptr;
	memLen = 0;
}

TBitFields::TBitFields(int len)
{
	if (len<0)
		ERROR("Error_size");
	else if (len == 0)
	{
		bitLen = 0;
		pMem = nullptr;
		memLen = 0;
	}
	else
	{
		bitLen = len;
		memLen = (len + BITSIZE - 1) / BITSIZE;
		pMem = new TELEM[memLen]{ 0 };
	}
}

TBitFields::TBitFields(const TBitFields& bf)
{
	if (bf.pMem)
	{
		bitLen = bf.bitLen;
		memLen = bf.memLen;
		pMem = new TELEM[memLen];
		for (int i = 0; i < memLen; ++i)
			pMem[i] = bf.pMem[i];
	}
	else
	{
		bitLen = 0;
		memLen = 0;
		pMem = nullptr;
	}
}

TBitFields::TBitFields(TBitFields&& bf) noexcept
{
	bitLen = bf.bitLen;
	memLen = bf.memLen;
	pMem = bf.pMem;
	bf.pMem = nullptr;
	bf.bitLen = 0;
	bf.memLen = 0;
}

TBitFields::~TBitFields()
{
	if (pMem)
	{
		delete[] pMem;
		pMem = nullptr;
		bitLen = 0;
		memLen = 0;
	}
}

int TBitFields::GetLength() const
{
	return bitLen;
}

void TBitFields::SetBit(const int n) {

	if (n < 0 || n >= bitLen)
		ERROR("Error_size");
	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] |= mask;
}

void TBitFields::ClrBit(const int n)
{
	if (n < 0 || n >= bitLen)
		ERROR("Error_size");
	int index = GetMemIndex(n);
	TELEM mask = ~GetMemMask(n);
	pMem[index] &= mask;
}

int TBitFields::GetBit(const int n) const
{
	if (n < 0 || n >= bitLen)
		ERROR("Error_size");
	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	return (pMem[index] & mask) != 0;
}

bool TBitFields::operator==(const TBitFields& bf) const
{
	if (memLen != bf.memLen || bitLen != bf.bitLen)
		return false;
	for (int i = 0; i < bitLen; ++i)
		if (this->GetBit(i) != bf.GetBit(i))
			return false;
	return true;
}

bool TBitFields::operator!=(const TBitFields& bf) const
{
	if (memLen != bf.memLen || bitLen != bf.bitLen)
		return true;
	for (int i = 0; i < bitLen; ++i)
		if (this->GetBit(i) != bf.GetBit(i))
			return true;
	return false;
}

TBitFields& TBitFields::operator=(const TBitFields& bf)
{
	if (*this != bf)
	{
		if (bf.pMem)
		{
			bitLen = bf.bitLen;
			memLen = bf.memLen;
			pMem = new TELEM[memLen];
			for (int i = 0; i < memLen; ++i)
				pMem[i] = bf.pMem[i];
		}
		else
		{
			bitLen = 0;
			memLen = 0;
			pMem = nullptr;
		}
		return *this;
	}
}

TBitFields& TBitFields::operator=(TBitFields&& bf) noexcept
{
	if (*this != bf)
	{
		bitLen = bf.bitLen;
		memLen = bf.memLen;
		pMem = bf.pMem;
		bf.pMem = nullptr;
		bf.bitLen = 0;
		bf.memLen = 0;
		return *this;
	}
}

TBitFields TBitFields::operator|(const TBitFields& bf)
{
	if (!bf.pMem)
		ERROR("rg == nullptr");
	if (!pMem)
		ERROR("lg == nullptr");
	TBitFields res((bitLen>bf.bitLen)?bitLen:bf.bitLen);
	for (int i = 0; i < ((memLen < bf.memLen) ? memLen : bf.memLen); ++i)
		res.pMem[i] = pMem[i] | bf.pMem[i];
	if (memLen > bf.memLen)
		for (int i = bf.memLen + 1; i < res.memLen; ++i)
			res.pMem[i] = pMem[i];
	else
		for (int i = memLen + 1; i < res.memLen; ++i)
			res.pMem[i] = bf.pMem[i];

	return TBitFields(res);
}

TBitFields TBitFields::operator&(const TBitFields& bf)
{
	if (!bf.pMem)
		ERROR("rg == nullptr");
	if (!pMem)
		ERROR("lg == nullptr");

	TBitFields res((bitLen > bf.bitLen) ? bitLen : bf.bitLen);
	int min = ((memLen < bf.memLen) ? memLen : bf.memLen);
	for (int i = 0; i < min; ++i)
		res.pMem[i] = pMem[i] & bf.pMem[i];
	return TBitFields(res);
}

TBitFields TBitFields::operator~()
{
	if (!pMem)
		ERROR("lg == nullptr");
	TBitFields res(bitLen);
	for (int i = 0; i < memLen; ++i)
		res.pMem[i] = ~pMem[i];
	TELEM mask = (static_cast<TELEM>(1) << (bitLen % BITSIZE)) - 1;
	res.pMem[memLen - 1] &= mask;
	return TBitFields(res);
}


std::istream& operator>>(std::istream& in, TBitFields& bf)
{
	int len;
	std::cout << "Enter count of bits: ";
	in >> len;

	if (len <= 0)
		ERROR("size error");

	TBitFields res(len);
	std::cout << "\nEnter bits: \n";

	for (int i = len - 1; i >= 0; i--) {
		char c;
		in >> c;

		if (c == '1') {
			res.SetBit(i);
		}
	}

	bf = std::move(res);
	return in;
}

std::ostream& operator<<(std::ostream& out, TBitFields& bf)
{
	out << "bitLen: " << bf.bitLen << '\n'
		<< "memLen: " << bf.memLen << '\n'
		<< "Field: " << '\n';
	for (int i = bf.bitLen-1; i >= 0;i--)
		out << bf.GetBit(i);
	out << '\n';
	return out;
}

std::ostream& operator<<(std::ostream& out, const TBitFields& bf)
{
	out << "bitLen: " << bf.bitLen << '\n'
		<< "memLen: " << bf.memLen << '\n'
		<< "Field: " << '\n';
	for (int i = bf.bitLen - 1; i >= 0; i--)
		out << bf.GetBit(i);
	out << '\n';
	return out;
}

