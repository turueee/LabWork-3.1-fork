// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля
#include <vector>
#include "tset.h"
#include "TError.h"

#define ERROR(err,...) throw TError(err, __func__, __FILE__, __LINE__)

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitFields FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : bitField(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet &s) : bitField(s.bitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitFields &bf) : bitField(bf)
{
}

TSet::operator TBitFields()
{
    return bitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return bitField.GetLength();
}

bool TSet::IsMember(const int Elem) const // элемент множества?
{
    return bitField.GetBit(Elem);
}

void TSet::Clean()
{
    bitField = TBitFields(this->GetMaxPower());
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    bitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    bitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (*this == s)
        return *this;
    bitField = s.bitField;
    return *this;
}

bool TSet::operator==(const TSet &s) const // сравнение
{
    return bitField == s.bitField;
}

bool TSet::operator!=(const TSet &s) const // сравнение
{
    return bitField != s.bitField;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    return bitField|s.bitField;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet set(*this);
    set.InsElem(Elem);
    return set;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet set(*this);
    set.DelElem(Elem);
    return set;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    return bitField & s.bitField;
}

TSet TSet::operator~() // дополнение
{
    return TSet(~bitField);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    std::vector<int> nums;
    int i;
    s.Clean();
    istr >> i;
    while (i != -1)
    {
        s.InsElem(i);
        istr >> i;
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << "{ ";
    for (int i = 0; i < s.GetMaxPower(); ++i)
        if (s.IsMember(i))
            ostr << i << ", ";
    ostr << "}";
    return ostr;
}
