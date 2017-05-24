// ShowPicDoc.cpp : implementation of the CShowPicDoc class
//

#include "stdafx.h"
#include "ShowPic.h"

#include "ShowPicDoc.h"
#include "ShowPicView.h"


//DECLARE_HANDLE(HDIB);
long intWidth;
long intHeight,size;
unsigned char Data[610][800];
unsigned char OriData[610][800];

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowPicDoc

IMPLEMENT_DYNCREATE(CShowPicDoc, CDocument)

BEGIN_MESSAGE_MAP(CShowPicDoc, CDocument)
	//{{AFX_MSG_MAP(CShowPicDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowPicDoc construction/destruction

CShowPicDoc::CShowPicDoc()
{
	// TODO: add one-time construction code here

}

CShowPicDoc::~CShowPicDoc()
{
}

BOOL CShowPicDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CShowPicDoc serialization

void CShowPicDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CShowPicDoc diagnostics

#ifdef _DEBUG
void CShowPicDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CShowPicDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CShowPicDoc commands
BOOL CShowPicDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	//�������

//	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);

	int x,y,k;
	FILE *fp = fopen(lpszPathName,"rb");

	fread(bfType,2,1,fp);


	if (bfType[0]=='B'&& bfType[1]=='M')
	{
		fread(&bf_h,12,1,fp);
		fread(&bi_h,40,1,fp);//��ȡ�ļ���Ϣͷ�ȹ�������
		intHeight=bi_h.biHeight;
		intWidth=bi_h.biWidth;
		size=intHeight*intWidth;
		//--------------------------------------24λ��ɫ----------------------------
		if(bi_h.biBitCount==24)
		{
			flag=1;
			for(y=intHeight-1;y>=0;y--)
			{
				for(x=0;x<bi_h.biWidth;x++)
					for(k=2;k>=0;k--)
						fread(&pic[y][x][k],1,1,fp);
					if(((intWidth*3)%4)!=0)
						fseek(fp,4-(intWidth*3)%4,1);
			}

		}


		//--------------------------------------8λ�Ҷ�----------------------------
		if(bi_h.biBitCount==8)
		{
			flag=2;
			if (!CDocument::OnOpenDocument(lpszPathName))
			return FALSE;			
			
				int i, j;

				CFile file;
				CFileException fileExce;
				BITMAPFILEHEADER bmpFH;
				BITMAPINFOHEADER bmpIH;
				DWORD dwSize;
				HDIB hDIB;
				LPSTR lpDIB;
								
				//���ļ�
				file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fileExce);
			/*
				if (m_hDIB == NULL)
				{
				// may not be DIB format
				return FALSE;
				}
			*/
				
				//��ȡ14�ֽڵ�FileHeader��40�ֽڵ�InfoHeader
				file.Read((LPSTR)&bmpFH,sizeof(bmpFH));
				file.Read((LPSTR)&bmpIH, sizeof(bmpIH));
				
				//ȡ�ļ��ܳ�
				dwSize = file.GetLength();
				
				//ȡͼ���͸�
			//	intWidth = bmpIH.biWidth;
				intHeight = bmpIH.biHeight;


				if(bmpIH.biWidth%4)
					intWidth=(bmpIH.biWidth+3)/4*4;
				else
					intWidth=bmpIH.biWidth;

////////////////////////////////////////////////////

				//���仺����
				hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize);
				lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

				//���ļ������ݽ�������
				file.ReadHuge(lpDIB, dwSize - sizeof(BITMAPFILEHEADER)- sizeof(BITMAPINFOHEADER));

				//������ɫ��
				lpDIB = lpDIB + 256*4;

					//���ļ�
				for(j = intHeight;j >= 0;j--)
					for(i = 0;i < intWidth; i++)
						Data[j][i] = *(lpDIB++);
		
				
			/*	for(j = 0;j < intHeight;j++)
				{
					for(i = 0;i < intWidth; i++)
					{
							Data[j][i] = *(lpDIB++);
					}
				}
			*/	
				file.Close();					  
		

				// �����ļ�����
				for(i = 0; i < intHeight; i++)
				{
				for(j = 0; j < intWidth; j++)
					{
						OriData[j][i] =Data[j][i];
					}
			}

				SetPathName(lpszPathName);

				::GlobalUnlock((HGLOBAL) hDIB);
				
				return TRUE;
	
		}
	}
//		::GlobalUnlock((HGLOBAL) m_hDIB);
		return TRUE;
}
