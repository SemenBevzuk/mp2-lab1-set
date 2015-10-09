// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	int bits;
	if (len<0)
	{
		throw invalid_argument("Len<0...");
	}
	bits = (len + sizeof(int) - 1) / sizeof(int);
	MemLen = bits;
	BitLen = len;
	pMem = new TELEM[BitLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	MemLen = bf.MemLen;
	BitLen = bf.BitLen;
	pMem = new TELEM[BitLen];
	for (int i=0; i < BitLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete []pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n/sizeof(TELEM);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{

	return 1 << n%sizeof(TELEM);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n<0)
	{
		throw invalid_argument("Bit < 0...");
	}
	if (n>BitLen)
	{
		throw invalid_argument("BitLen < n...");
	}
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n<0) {
		throw invalid_argument("Bit < 0...");
	}
	if (n>BitLen) {
		throw invalid_argument("BitLen < n...");
	}
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n<0)
	{
		throw invalid_argument("n < 0...");
	}
	if (n>BitLen)
	{
		throw invalid_argument("BitLen < n...");
	}
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	/*if (MemLen != bf.MemLen)
	{
		throw "Length 1 != Length 2";
	}*/
	if(pMem != bf.pMem)
	{
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		delete []pMem;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
		{
			pMem[i] = bf.pMem[i];
		}
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int result=1;
	if (BitLen == bf.BitLen)
	{
		for (int i = 0; i < BitLen;i++)
		{
			if (GetBit(i)!=bf.GetBit(i))
			{
				result = 0;
				return result;
			}
		}
	}
	else
	{
		result = 0;
	}
  return result;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (*this == bf)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{	
	int max_len;
	if (BitLen >= bf.BitLen)
	{
		max_len = BitLen;
	}
	else
	{
		max_len = bf.BitLen;
	}
	TBitField result(max_len);
	for (int i = 0; i < MemLen; i++)
	{
		result.pMem[i] = pMem[i];
	}
	for (int i = 0; i < bf.MemLen; i++)
	{
		result.pMem[i]  = result.pMem[i] | bf.pMem[i];
	}
	return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int max_len;
	if (BitLen >= bf.BitLen) {
		max_len = BitLen;
	}
	else {
		max_len = bf.BitLen;
	}
	TBitField result(max_len);
	for (int i = 0; i < MemLen; i++) {
		result.pMem[i] = pMem[i];
	}
	for (int i = 0; i < bf.MemLen; i++) {
		result.pMem[i] = result.pMem[i] & bf.pMem[i];
	}
	return result;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField result(BitLen);
	for (int i = 0; i < MemLen; i++)
	{
		result.pMem[i] = ~pMem[i];
	}
	return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	// как последовательность 0 и 1
	char c;
	int i = 0;
	istr >> c;
	while ((c == '1') || (c == '0'))
	{
		if (c == '1')
		{
			bf.SetBit(i);
			i++;
		}
		else
		{
			bf.ClrBit(i);
			i++;
		}
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	ostr << "Length = " << bf.BitLen << endl;
	for (int i = 0; i < bf.BitLen; i++)
	{
		ostr << bf.GetBit(i);
	}
	return ostr;
}
