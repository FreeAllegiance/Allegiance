/*-------------------------------------------------------------------------
 * CBitArray
 *-------------------------------------------------------------------------
 * Purpose:
 *    Constructor
 * 
 * Parameters:
 *    number of bits in each dimension (square)
 * 
 * Side Effects:
 *    allocates grid
 */

#include "pch.h"

CBitArray::CBitArray(int nBits) :
  m_nMin(0)
{
  CBitArray(0, nBits);
}

CBitArray::CBitArray(int nBitsMin, int nBitsMax) // minimum is inclusive, maximum is exclusive
{
  m_nMin = nBitsMin;
  m_nBits = nBitsMax - nBitsMin;
  m_cElemsPerRow = (m_nBits - 1) / (8 * sizeof (*m_pargBits)) + 1;
  m_pargBits = (unsigned int *) GlobalAllocPtr(GMEM_MOVEABLE | GMEM_ZEROINIT,
                                              m_cElemsPerRow * sizeof (*m_pargBits) * m_nBits);
}

CBitArray::~CBitArray()
{
  GlobalFreePtr(m_pargBits);
  m_pargBits = NULL;
}


/*-------------------------------------------------------------------------
 * FIsSet
 *-------------------------------------------------------------------------
 * Purpose:
 *    Tells you whether the bit is set
 * 
 * Returns:
 *    whether the bit is set
 */
bool CBitArray::FIsSet(int x, int y)
{
  x -= m_nMin;
  y -= m_nMin;
  int yInt = y / (8 * sizeof (*m_pargBits));
  return 0 != (*(m_pargBits + m_cElemsPerRow * x + yInt) & 
               ((unsigned int) 1 << (y - (8 * sizeof (*m_pargBits)) * yInt)));
}


/*-------------------------------------------------------------------------
 * Set
 *-------------------------------------------------------------------------
 * Purpose:
 *    set (or clear) a bit in the table
 * 
 * Returns:
 *    the value of bit after the set (the parameter b), just for convenience in expressions
 */
bool CBitArray::Set(int x, int y, bool b)
{
  x -= m_nMin;
  y -= m_nMin;
  int yInt = y / (8 * sizeof (*m_pargBits));

  if (b)
    *(m_pargBits + m_cElemsPerRow * x + yInt) |=  ((unsigned int) 1 << (y - (8 * sizeof (*m_pargBits)) * yInt));
  else
    *(m_pargBits + m_cElemsPerRow * x + yInt) &= ~((unsigned int) 1 << (y - (8 * sizeof (*m_pargBits)) * yInt));

  return b;
}


/*-------------------------------------------------------------------------
 * ClearRow
 *-------------------------------------------------------------------------
 * Purpose:
 *    Zero out selected row
 */
void CBitArray::ClearRow(int x)
{
  x -= m_nMin;
  ZeroMemory(m_pargBits + m_cElemsPerRow * x, m_cElemsPerRow * sizeof (*m_pargBits));
}


/*-------------------------------------------------------------------------
 * ClearColumn
 *-------------------------------------------------------------------------
 * Purpose:
 *    Zero out selected column
 */
void CBitArray::ClearColumn(int y)
{
  int x;
  y -= m_nMin;
  int yInt = y / (8 * sizeof (*m_pargBits));
  for (x = 0; x < m_nBits; x++)
    *(m_pargBits + m_cElemsPerRow * x + yInt) &=  ~((unsigned int) 1 << (y - (8 * sizeof (*m_pargBits)) * yInt));
}

void CBitArray::Dump(int min, int max)
{
  ZAssert (max >= min);
  int x, y;
  int iChar;
  int cRowsCols = max - min + 1;
  int nColSize;
  if (min >=0 && max < 10)
    nColSize = 1;
  else if (min > -10 && max < 100)
    nColSize = 2;
  else 
    nColSize = 3;

  // draw the header
  for (iChar = 0; iChar < nColSize; iChar++)
    debugf(" ");
  debugf("|");
  for (y = min; y <= max; y++)
    debugf("%*d", nColSize, y);
  debugf("\n");
  for (iChar = 0; iChar < nColSize; iChar++)
    debugf("-");
  debugf("+");
  for (y = min; y <= max * nColSize; y++)
    debugf("-");
  debugf("\n");

  // Draw each row
  for (x = min; x <= max; x++)
  {
    debugf("%*d", nColSize, x);
    debugf("|");
    for (y = min; y <= max; y++)
      debugf("%*d", nColSize, FIsSet(x, y));
    debugf("\n");
  }  
  debugf("\n");
}
