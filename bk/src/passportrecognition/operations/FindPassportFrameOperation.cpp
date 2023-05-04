#include "stdafx.h"
#include "FindPassportFrameOperation.h"
#include "RegressionLine.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef M_PI 
#define M_PI 3.14159265358979323846
#endif

#define FROM_LEFT_START_SEARCH_AFTER 550 // 5.5cm
#define FROM_TOP_START_SEARCH_AFTER 500 //5cm
#define CHECK_EDGE_EVERY 200 //2cm
#define FINISH_SEARCH_AFTER 200 //2cm
#define PROBE_TOP_EDGE_TIMES 5 // 5 times
#define PROBE_LEFT_EDGE_TIMES 4 //4 times
#define MAX_SKEW_DEG 5.0
#define MIN_LINE_THIKNESS (2)
#define PROBE_STEP_WIDTH (5) 
#define ZERO_ANGLE_EPS (0.5*(M_PI/180))

#define SWAP_GRAY(a, b) { BYTE t; t = b; b = a; a = t; }
#define SWAP_SLICE(a, b) { BYTE *t; t = b; b = a; a = t; }

#if _DEBUG
#define SAVEDEBUGIMGS
#endif


void FindPassportFrameOperation::DoOperation(PassportImageObject* passportImgObj)
{
	fipWinImage median;
	fipWinImage edgeDetect;
	
	int dpiResolution = passportImgObj->m_dpiResolution.ToInt(0);

	const int flssa_du = (FROM_LEFT_START_SEARCH_AFTER*dpiResolution) / 254,
		ftssa_du = (FROM_TOP_START_SEARCH_AFTER*dpiResolution) / 254,
		cee_du = (CHECK_EDGE_EVERY*dpiResolution) / 254,
		fsa_du = (FINISH_SEARCH_AFTER*dpiResolution) / 254;

	//tutaj assery rozmiaru
	
	passportImgObj->m_PreparedPassportImage.copySubImage(edgeDetect, flssa_du, 0, flssa_du + (PROBE_TOP_EDGE_TIMES)*CHECK_EDGE_EVERY, fsa_du);
		
	Median(edgeDetect,median);

	int i,j,ttimes,ltimes,lineThikness;

	double topA,topB,leftA,leftB;
	bool bFound;

	BYTE *pLine,*pLineProbe1,*pLineProbe2,*pLineProbe3;

	RegressionLine::Points pnts;
	pnts.clear();

	int probe1,probe2,probe3;

	int imgx =  median.getWidth(),
		imgy = median.getHeight();
	
	for(i = PROBE_STEP_WIDTH + 1,ttimes = 0;i < imgx && ttimes < PROBE_TOP_EDGE_TIMES;i += cee_du,ttimes++)
	{
		j = 0;
		while(j < (fsa_du) && j < imgy)
		{
			pLine = median.getScanLine(imgy-j-1);
			if(pLine[i] > 0) 
				break;
			j++;
		}
				
		bFound = false;
		lineThikness = 0;
		for(; j < (fsa_du) && j < imgy;j++) 
		{
			pLine = median.getScanLine(imgy-j-1);

			probe1 = pLine[i-PROBE_STEP_WIDTH] <= 0 ? 1 : 0;
			probe2 = pLine[i] <= 0 ? 1 : 0;
			probe3 = pLine[i+PROBE_STEP_WIDTH] <= 0 ? 1 : 0;
			
			pLine[i-PROBE_STEP_WIDTH] = 0;
			pLine[i] = 0;
			pLine[i+PROBE_STEP_WIDTH] = 0;
			
			
			if(probe1+probe2+probe3 >= 2) //at least two probes found line
			{
				lineThikness++;
				if(lineThikness >= MIN_LINE_THIKNESS)
				{
					pnts[(double)i] = -(double)(j);
					bFound = true;
					//TRACE("TOP PROBE %d,AT %d,  RESULT = %d\n",ttimes,i,j);
					break;
				}
			}


		}
		if(!bFound) break; //didnt find all points
	}
	
	
	if (ttimes != PROBE_TOP_EDGE_TIMES) return;
	
	RegressionLine topLine(pnts);

	//TRACE("Top regresion coef = %f\r\n",topLine.regressionCoefficient());

	topA = topLine.slope();
	topB = topLine.yIntercept();

	//TRACE("TOP ANGLE %f\r\n",(atan(topA)*180)/M_PI);
	//TRACE("TOP A = %f,TOP B = %f\n",topA,topB);

	pnts.clear();

	passportImgObj->m_PreparedPassportImage.copySubImage(edgeDetect, 0, ftssa_du, fsa_du, ftssa_du + (PROBE_LEFT_EDGE_TIMES)*CHECK_EDGE_EVERY);
	
	Median(edgeDetect,median);

	imgx =  median.getWidth();
	imgy = median.getHeight();


	for(j = PROBE_STEP_WIDTH + 1,ltimes = 0;j < imgy && ltimes < PROBE_LEFT_EDGE_TIMES;j += cee_du,ltimes++)
	{
		pLineProbe1 = median.getScanLine(imgy-j-1-PROBE_STEP_WIDTH);
		pLineProbe2 = median.getScanLine(imgy-j-1);
		pLineProbe3 = median.getScanLine(imgy-j-1+PROBE_STEP_WIDTH);
		
		
		i = 0;
		while(i < (fsa_du) && i < imgx)
		{
			if(pLineProbe2[i] > 0)
				break;
			i++;
		}

		//TRACE("Skip left %d black pixels\r\n",i);
		bFound = false;
		lineThikness = 0;
		for(;i < (fsa_du) && i < imgx;i++)
		{

			probe1 = pLineProbe1[i] <= 0 ? 1 : 0;
			probe2 = pLineProbe2[i] <= 0 ? 1 : 0;
			probe3 = pLineProbe3[i] <= 0 ? 1 : 0;


			pLineProbe1[i] = 0;
			pLineProbe2[i] = 0;
			pLineProbe3[i] = 0;


			 
			if(probe1+probe2+probe3 >= 2)
			{
				lineThikness++;
				if(lineThikness >= MIN_LINE_THIKNESS)
				{
					pnts[(double)j] = (double)(i);
					bFound = true;
	//				TRACE("LEFT PROBE %d,AT %d,  RESULT = %d\n",ltimes,j,i);
					break;
				}
			}
		}
		if(!bFound) break;

	}
	if (ltimes != PROBE_LEFT_EDGE_TIMES) return;

	//SaveDebugImage(L"median_probesLeft",median);

	RegressionLine leftLine(pnts);

	//TRACE("Left regresion coef = %f\r\n",leftLine.regressionCoefficient());

	leftA = leftLine.slope();
	leftB = leftLine.yIntercept();

	double leftAngle = atan(leftA),topAngle = atan(topA);
	double dSkew = 0;
	if(topAngle < ZERO_ANGLE_EPS && topAngle > (-ZERO_ANGLE_EPS))
		dSkew = 0.0;
	else
		dSkew = topAngle;

	int orgX, orgY;

	if(leftAngle < ZERO_ANGLE_EPS && leftAngle > (-ZERO_ANGLE_EPS))
	{
		orgX = (int)(leftB);
		orgY = -(int)(topA*leftB + topB);
	}
	else
	{
		leftB = -leftB/leftA;
		leftA = 1/leftA;

		orgX = (int)((leftB - topB) / (topA - leftA));
		orgY = -(int)(topA*orgX + topB);
	}

	passportImgObj->m_Org = Point<int>(orgX, orgY);
	passportImgObj->m_Skew = dSkew;
}

void FindPassportFrameOperation::Median(fipWinImage &inImage, fipWinImage &outImage)
{
	int cols = inImage.getWidth(), rows = inImage.getHeight();
	int col, row;

	outImage.setSize(inImage.getImageType(), cols, rows, inImage.getBitsPerPixel());

	// just copy border pixels to output
	for (col = 0; col < cols; col++)
	{
		outImage.getScanLine(0)[col] = inImage.getScanLine(0)[col];
		outImage.getScanLine(rows - 1)[col] = inImage.getScanLine(rows - 1)[col];
	}

	for (row = 1; row < rows - 1; row++)
	{
		outImage.getScanLine(row)[0] = inImage.getScanLine(row)[0];
		outImage.getScanLine(row)[cols - 1] = inImage.getScanLine(row)[cols - 1];
	}

	BYTE _a[3], _b[3], _c[3], _d[3], bc[6];
	BYTE *a = _a, *b = _b, *c = _c, *d = _d,
		*scanL1, *scanL2, *scanL3, *oscanL;


	for (row = 1; row < rows - 1; row++)
	{
		scanL1 = inImage.getScanLine(row - 1);
		scanL2 = inImage.getScanLine(row);
		scanL3 = inImage.getScanLine(row + 1);

		a[0] = scanL1[0];
		a[1] = scanL2[0];
		a[2] = scanL3[0];
		SORT_SLICE(a);

		b[0] = scanL1[1];
		b[1] = scanL2[1];
		b[2] = scanL3[1];

		SORT_SLICE(b);

		for (col = 1; col < cols - 1; col += 2)
		{

			c[0] = scanL1[col + 1];
			c[1] = scanL2[col + 1];
			c[2] = scanL3[col + 1];

			d[0] = scanL1[col + 2];
			d[1] = scanL2[col + 2];
			d[2] = scanL3[col + 2];

			// sort slice c and d 
			// slice a and b are already sorted

			SORT_SLICE(c);
			SORT_SLICE(d);

			// merge slice b and c producing slice bc
			MERGE_SLICE(b, c, bc);

			oscanL = outImage.getScanLine(row);
			// merge slice bc and a to calculate median 1
			oscanL[col] = CALC_MEDIAN(a, bc);

			// merge slice bc and d to calculate median 2
			oscanL[col + 1] = CALC_MEDIAN(d, bc);

			SWAP_SLICE(a, c);
			SWAP_SLICE(b, d);
		}
	}
}

void FindPassportFrameOperation::SORT_SLICE(BYTE a[])
{
	if (a[0] > a[1]) SWAP_GRAY(a[0], a[1]);
	if (a[1] > a[2]) SWAP_GRAY(a[1], a[2]);
	if (a[0] > a[1]) SWAP_GRAY(a[0], a[1]);
}

void FindPassportFrameOperation::MERGE_SLICE(BYTE a[], BYTE b[], BYTE bc[])
{
	int i, j, k;

	for (i = 0, j = 0, k = 0; i < 6; i++)
	if (j > 2)
		bc[i] = b[k++];
	else if (k > 2)
		bc[i] = a[j++];
	else
		bc[i] = (a[j] < b[k]) ? a[j++] : b[k++];
}

BYTE FindPassportFrameOperation::CALC_MEDIAN(BYTE x[], BYTE bc[])
{
	int i, j, k, m;

	for (i = 0, j = 0, k = 1; i < 4; i++)
	if (j > 2)
		m = bc[k++];
	else
		m = (x[j] < bc[k]) ? x[j++] : bc[k++];

	return m;
}

