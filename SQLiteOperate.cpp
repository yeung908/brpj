
#include "stdafx.h"
#include "SQLiteOperate.h"


CSqliteOperate::CSqliteOperate(void)
{
	state=false;
}

CSqliteOperate::~CSqliteOperate(void)
{
   DisConnectionDataBase();
}


//读数据库配置文件
bool CSqliteOperate::ReadFile(TCHAR* FileName)
{
	TCHAR temp[256]="";
	TCHAR tempchar[256]="";

	FILE *fp=_tfopen(FileName,_T("r"));
	if(fp)
	{
		_fgetts(temp,256,fp);
		_stscanf(temp,_T("Ip:%s"),Ip);

		_fgetts(temp,256,fp);
		_stscanf(temp,_T("Port:%s"),Port);

		_fgetts(temp,256,fp);
		_stscanf(temp,_T("User:%s"),User);

		_fgetts(temp,256,fp);
		_stscanf(temp,_T("Psw:%s"),Psw);

		_fgetts(temp,256,fp);
		_stscanf(temp,_T("DataBaseName:%s"),DataBaseName);

		fclose(fp);

		return TRUE;
	}
	else
		return FALSE;
}

//连接数据库
int CSqliteOperate::ConnectionDataBase(char*  FileName)
{
	AfxOleInit();
	CoInitialize(NULL);

	//ReadConfigTxt pConfig;
	if(!ReadFile(FileName))
	{
		return ReadFile_FAIL;
	}

	CString CstrConn;
#if 0
	CstrConn.Format(_T("Provider=OraOLEDB.Oracle.1;User ID=%s;Password=%s;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=%s)(PORT=%s))(CONNECT_DATA=(SERVICE_NAME=%s)));Persist Security Info=False"),\
					User, Psw, Ip, Port, DataBaseName);
#else
	CstrConn.Format(_T("Provider=SQLOLEDB.1;Persist Security Info=False;Server=%s,%s;Database=%s; uid=%s; pwd=%s"),\
						Ip, Port, DataBaseName, User, Psw);
#endif
	HRESULT hr;
	if(SUCCEEDED(m_pConnection.CreateInstance("ADODB.Connection")))
	{
		_bstr_t strConnect = _bstr_t(CstrConn);

		m_pConnection->ConnectionTimeout = 30;

		try
		{
			hr = m_pConnection->Open(strConnect,"","",adModeUnknown);
		}
		catch(_com_error e)
		{
			return ContOpen_FAIL;
		}

		state=true;
		return Connectd_DONE;
	}
	else
	{
		return Instance_FAIL;
	}
}

//断开与oracle数据库的连接
bool CSqliteOperate::DisConnectionDataBase(void)
{
	try
	{
		if(m_pConnection->State)
		{
			m_pConnection->Close();
			state=false;
			return true;
		}
	}
	catch(_com_error e)        //捕捉异常
	{
		CString temp;
		temp.Format(_T("错误信息:%s"),e.ErrorMessage());
		return false;
	}	

	return false;
}


////////////////////////////////////////////////////
//摄像头表
void CSqliteOperate::Camera_CreateTable(void)
{
	CString strsql;
	strsql = _T("create table tb_camera(nid number primary key,sname varchar2(24), \
				 sip varchar2(40), suser varchar2(20), \
				 spsw varchar2(20), nport number)");
	m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
}

////////////////////////////////////////////////////
void CSqliteOperate::Camera_Add(struct CAMERA_INFO_ST cInfo)
{
	CString strsql;

	strsql.Format("insert into tb_camera(sname,sip,suser,spsw,nport) values('%s', '%s', '%s', '%s', %d)",\
				   cInfo.name,cInfo.ip,cInfo.user,cInfo.psw,cInfo.port);
	m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
}

////////////////////////////////////////////////////
void CSqliteOperate::Camera_Modify(struct CAMERA_INFO_ST cInfo)
{
	CString strsql;
	strsql.Format("update tb_camera set sname='%s',sip='%s',suser='%s',spsw='%s',nport=%d where nid=%d",\
				   cInfo.name,cInfo.ip,cInfo.user,cInfo.psw,cInfo.port,cInfo.nid);
	m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
}

////////////////////////////////////////////////////
void CSqliteOperate::Camera_Delete(unsigned long int nid)
{
	CString strsql;
	strsql.Format("delete from tb_camera where nid=%d",nid);
	m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
}

////////////////////////////////////////////////////
bool CSqliteOperate::Camera_Read(int maxNum,list<struct CAMERA_INFO_ST> &cameraList)
{
	CString strSql = _T("select * from tb_camera");
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	if(m_pRecordsetPtr->adoBOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}
	while(!m_pRecordsetPtr->adoEOF)
	{
		variant_t temp; 
		temp.ChangeType(VT_NULL);

		variant_t nid = m_pRecordsetPtr->GetCollect("nid");
		variant_t camName = m_pRecordsetPtr->GetCollect("sname");
		variant_t sip = m_pRecordsetPtr->GetCollect("sip");
		variant_t user = m_pRecordsetPtr->GetCollect("suser");
		variant_t password = m_pRecordsetPtr->GetCollect("spsw");
		variant_t port = m_pRecordsetPtr->GetCollect("nport");

		struct CAMERA_INFO_ST CameraInfo = {0};
		CameraInfo.nid = nid;
		CameraInfo.port = port;

		if(camName.vt != NULL && camName !=temp)
		{
			CString str = camName.bstrVal;
			strcpy(CameraInfo.name, str.GetBuffer(0));
		}
		if(sip.vt != NULL && sip !=temp)
		{
			CString str = sip.bstrVal;
			strcpy(CameraInfo.ip, str.GetBuffer(0));
		}
		if(user.vt != NULL && user !=temp)
		{
			CString str = user.bstrVal;
			strcpy(CameraInfo.user, str.GetBuffer(0));
		}
		if(password.vt != NULL && password !=temp)
		{
			CString str = password.bstrVal;
			strcpy(CameraInfo.psw, str.GetBuffer(0));
		}

		cameraList.push_back(CameraInfo);

		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();
	if(cameraList.size())
	{
		return true;
	}
	else
	{
		return false;
	}
}

////////////////////////////////////////////////////
//产品信息表
void CSqliteOperate::Product_CreateTable(void)
{
	CString strsql;
	strsql = _T("create table tb_product(nid number primary key,RunningNumber varchar2(256), \
				 tag varchar2(256), MainCategory varchar2(256), \
				 SubCategory varchar2(256), MetaField varchar2(256), \
				 ColourDesc varchar2(256), Colour varchar2(256), \
				 Unit varchar2(256), FactoryItem varchar2(256), \
				 HmNum varchar2(256), Description varchar2(2560), \
				 path1 varchar2(260), path2 varchar2(260), \
				 path3 varchar2(260))");
	m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
}

////////////////////////////////////////////////////
void CSqliteOperate::Product_Add(struct PRODUCT_INFO_ST pInfo)
{
	CString strsql;
	strsql.Format("insert into tb_product(RunningNumber,tag,MainCategory,SubCategory,\
										MetaField,ColourDesc,Colour,Unit,FactoryItem,\
										HmNum,Description,path1,path2,path3) \
										values('%s', '%s', '%s', '%s', '%s', '%s', '%s', \
										'%s', '%s', '%s', '%s', '%s', '%s', '%s')",\
										pInfo.RunningNumber,pInfo.tag,pInfo.MainCategory,pInfo.SubCategory,\
										pInfo.MetaField,pInfo.ColourDesc,pInfo.Colour,pInfo.Unit,pInfo.FactoryItem,\
										pInfo.HmNum,pInfo.Description,pInfo.path1,pInfo.path2,pInfo.path3);
//	strsql = _T("delete from tb_product");
	m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
}

////////////////////////////////////////////////////
bool CSqliteOperate::Product_Read(char *barcode,struct PRODUCT_INFO_ST &pInfo)
{
	CString strSql;
	strSql.Format(_T("select * from tb_product where RunningNumber='%s' or tag = '%s'"),barcode,barcode);
	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	if(m_pRecordsetPtr->adoBOF)
	{
		return false;
	}

	variant_t var;
	variant_t temp; 
	temp.ChangeType(VT_NULL);
	CString str;

	pInfo.nid = (long)m_pRecordsetPtr->GetCollect("nid");

	var = m_pRecordsetPtr->GetCollect("RunningNumber");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(pInfo.RunningNumber, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("tag");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(pInfo.tag, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("MainCategory");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(pInfo.MainCategory, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("SubCategory");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(pInfo.SubCategory, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("MetaField");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(pInfo.MetaField, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("ColourDesc");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(pInfo.ColourDesc, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("Colour");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(pInfo.Colour, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("Unit");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(pInfo.Unit, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("FactoryItem");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(pInfo.FactoryItem, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("HmNum");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(pInfo.HmNum, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("Description");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(pInfo.Description, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("path1");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(pInfo.path1, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("path2");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(pInfo.path2, str.GetBuffer(0));
	}
	var = m_pRecordsetPtr->GetCollect("path3");
	if(var.vt != NULL && var !=temp)
	{
		str = var.bstrVal;
		strcpy(pInfo.path3, str.GetBuffer(0));
	}

	m_pRecordsetPtr->Close();

	return true;
}

////////////////////////////////////////////////////
//视频信息表
void CSqliteOperate::Video_CreateTable(void)
{
	CString strsql;
	strsql = _T("create table tb_video(nid number primary key,RunningNumber varchar2(256), \
				 tag varchar2(256), HmNum varchar2(256), Description varchar2(2560), \
				 stime date, etime date, nsize number, spath varchar2(260))");
	m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
}

////////////////////////////////////////////////////
void CSqliteOperate::Video_Add(char *RunningNumber,char *tag,char *HmNum,char *Description,\
							   char *starttime,char *endtime,char *path,unsigned long size)
{
	CString strsql;

	strsql.Format("insert into tb_video(RunningNumber,tag,HmNum,Description,stime,etime,nsize,spath) \
					values('%s','%s','%s','%s','%s','%s',%d,'%s')",\
					RunningNumber,tag,HmNum,Description,starttime,endtime,size,path);
	m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
}

////////////////////////////////////////////////////
void CSqliteOperate::Video_Delete(unsigned long int nid)
{
	CString strsql;
	strsql.Format("delete from tb_video where nid=%d",nid);
	m_pConnection->Execute((_bstr_t)strsql, NULL, adCmdText);
}

////////////////////////////////////////////////////
unsigned long CSqliteOperate::Video_GetCount(char *runnum,char *HmNum,char *Description,\
											 char *starttime,char *endtime,int flag,char *sql)
{
	CString strSql = _T("select count(*) as number from tb_video");
	CString tempSql = "";
	CString outSql = "";
	bool wflag = false;

	if((flag&0x01) != 0)
	{	
		tempSql.Format(_T(" where RunningNumber like '%%s%' or tag like '%%s%'"),runnum,runnum);
		wflag = true;
		strSql = strSql + tempSql;
		outSql = outSql + tempSql;
	}
	if((flag&0x02) != 0)
	{
		if(wflag)
		{
			tempSql.Format(_T(" and HmNum like '%%s%'"),HmNum);
		}
		else
		{
			tempSql.Format(_T(" where HmNum like '%%s%'"),HmNum);
			wflag = true;
		}
		strSql = strSql + tempSql;
		outSql = outSql + tempSql;
	}
	if((flag&0x04) != 0)
	{
		if(wflag)
		{
			tempSql.Format(_T(" and Description like '%%s%'"),Description);
		}
		else
		{
			tempSql.Format(_T(" where Description = '%%s%'"),Description);
			wflag = true;
		}
		strSql = strSql + tempSql;
		outSql = outSql + tempSql;
	}
	if((flag&0x08) != 0)
	{
		if(wflag)
		{
			tempSql.Format(_T(" and stime >= '%s' and etime <= '%s'"),starttime,endtime);
		}
		else
		{
			tempSql.Format(_T(" where stime >= '%s' and etime <= '%s'"),starttime,endtime);
			wflag = true;
		}
		strSql = strSql + tempSql;
		outSql = outSql + tempSql;
	}
	strcpy(sql,outSql.GetBuffer(0));

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql,NULL,adCmdText);

	unsigned long num = (unsigned long)(long)m_pRecordsetPtr->GetCollect("number");

	return num;
}

bool CSqliteOperate::Video_GetHistory(char *sql,int flag,int startNum,int endNum,\
									  list<struct VIDEO_INFO_ST> &videoList)
{
	CString strSql = "select * from (select nid,RunningNumber,tag,HmNum,Description,\
									 convert(varchar(19),stime,120) as starttime,\
									 convert(varchar(19),etime,120) as endtime,\
									 nsize,spath,row_number() over(order by nid desc) as rn from tb_video";
	CString tempSql = sql;
	CString numSql;
	numSql.Format(_T(") as a where rn >= %d and rn <= %d"),startNum,endNum);

	strSql = strSql + tempSql + numSql;

	m_pRecordsetPtr = m_pConnection->Execute((_bstr_t)strSql, NULL, adCmdText);
	if(m_pRecordsetPtr->adoBOF)
	{
		return false;
	}
	else
	{
		m_pRecordsetPtr->MoveFirst();
	}

	while(!m_pRecordsetPtr->adoEOF)
	{
		variant_t temp; 
		temp.ChangeType(VT_NULL);

		variant_t nid = m_pRecordsetPtr->GetCollect("nid");
		variant_t RunningNumber = m_pRecordsetPtr->GetCollect("RunningNumber");
		variant_t tag = m_pRecordsetPtr->GetCollect("tag");
		variant_t HmNum = m_pRecordsetPtr->GetCollect("HmNum");
		variant_t Description = m_pRecordsetPtr->GetCollect("Description");
		variant_t stime = m_pRecordsetPtr->GetCollect("starttime");
		variant_t etime = m_pRecordsetPtr->GetCollect("endtime");
		variant_t nsize = m_pRecordsetPtr->GetCollect("nsize");
		variant_t spath = m_pRecordsetPtr->GetCollect("spath");

		struct VIDEO_INFO_ST VideoInfo = {0};
		VideoInfo.nid = (long)nid;
		if(RunningNumber.vt != NULL && RunningNumber !=temp)
		{
			CString str = RunningNumber.bstrVal;
			strcpy(VideoInfo.RunningNumber, str.GetBuffer(0));
		}
		if(tag.vt != NULL && tag !=temp)
		{
			CString str = tag.bstrVal;
			strcpy(VideoInfo.tag, str.GetBuffer(0));
		}
		if(HmNum.vt != NULL && HmNum !=temp)
		{
			CString str = HmNum.bstrVal;
			strcpy(VideoInfo.HmNum, str.GetBuffer(0));
		}
		if(Description.vt != NULL && Description !=temp)
		{
			CString str = Description.bstrVal;
			strcpy(VideoInfo.Description, str.GetBuffer(0));
		}

		sscanf(LPCTSTR((CString)stime.bstrVal),"%04d-%02d-%02d %02d:%02d:%02d",
			   &VideoInfo.start_year,&VideoInfo.start_mon,&VideoInfo.start_day,
			   &VideoInfo.start_day,&VideoInfo.start_min,&VideoInfo.start_sec);
		sscanf(LPCTSTR((CString)etime.bstrVal),"%04d-%02d-%02d %02d:%02d:%02d",
			   &VideoInfo.end_year,&VideoInfo.end_mon,&VideoInfo.end_day,
			   &VideoInfo.end_day,&VideoInfo.end_min,&VideoInfo.end_sec);

		VideoInfo.size = (long)nsize;
		if(spath.vt != NULL && spath !=temp)
		{
			CString str = spath.bstrVal;
			strcpy(VideoInfo.path, str.GetBuffer(0));
		}

		videoList.push_back(VideoInfo);
		m_pRecordsetPtr->MoveNext();
	}
	m_pRecordsetPtr->Close();
	if(videoList.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;

}