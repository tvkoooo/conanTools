#include "myfile.h"
#include <fstream>

#define CMYCONFIGTABLE_TITLE_LINE 1

CMyConfigVec::CMyConfigVec(const char* file_name)
{
	m_pv = new typeV();
	std::ifstream file_confg(file_name);
	if (file_confg.is_open())
	{
		do
		{
			std::shared_ptr<std::string> pstr = std::make_shared<std::string>();
			if (!std::getline(file_confg, *pstr))
			{
				break;
			}
			m_pv->push_back(pstr);
		} while (true);
		file_confg.close();
	}
}
CMyConfigVec::~CMyConfigVec()
{
	m_pv->clear();
	delete m_pv;
}

CMyConfigKv::CMyConfigKv(const char* file_name)
{
	m_pv = new typeV();
	std::ifstream file_confg(file_name);
	if (file_confg.is_open())
	{
		do
		{
			std::string s;
			if (!std::getline(file_confg, s))
			{
				break;
			}
			bool bf = false;
			char* ck = &s[0];
			char* cv = NULL;
			for (uint32_t i = 0; i < s.size(); ++i)
			{
				if (s[i] == '=')
				{
					s[i] = 0;
					cv = &s[i+1];
					bf = true;
					break;
				}
			}
			if (bf)
			{
				(*m_pv)[ck] = cv;
			}
		} while (true);
		file_confg.close();
	}
}
CMyConfigKv::~CMyConfigKv()
{
	m_pv->clear();
	delete m_pv;
}
const char* CMyConfigKv::getConfigKv(const char* k)const
{
	typeV::const_iterator it = m_pv->find(k);
	if (it != m_pv->end())
	{
		return it->second.c_str();
	}
	return NULL;
}


CMyConfigTable::CMyConfigTable(const char* file_name)
{
	m_pData = new std::string();
	m_pv = new typeV();
	m_pr = new typeRow();
	m_pl = new typeLine();
	std::ifstream file_confg(file_name);
	if (file_confg.is_open())
	{
		do
		{
			file_confg.seekg(0, std::ios_base::end);
			auto size = file_confg.tellg();
			file_confg.seekg(0);
			m_pData->resize(static_cast<size_t>(size));
			file_confg.read(&((*m_pData)[0]), static_cast<std::streamsize>(size));
			file_confg.close();
			printf("CMyConfigTable size:%d\n", m_pData->size());
			m_iMaxRow = 0;
			m_iMaxLine = 0;
			int32_t pos = 0;
			int32_t line = 0;
			int32_t row = 0;
			const char* last = &((*m_pData)[0]);
			(*m_pv)[pos] = last;
			for (size_t i = 0; i < size; i++)
			{
				if ((*m_pData)[i] == '\t')
				{
					(*m_pData)[i] = 0;
					if (row == 0 && line > CMYCONFIGTABLE_TITLE_LINE)
					{
						(*m_pl)[last] = line;
					}
					if (line == CMYCONFIGTABLE_TITLE_LINE)
					{
						(*m_pr)[last] = row;
					}
					++row;
					pos = line * m_iMaxRow + row;
					last = &((*m_pData)[i + 1]);
					(*m_pv)[pos] = last;
				}
				else if ((*m_pData)[i] == '\n')
				{
					(*m_pData)[i] = 0;
					++line;

					if (m_iMaxRow == 0)
					{
						m_iMaxRow = row + 1;
					}
					else
					{
						if (m_iMaxRow != row + 1)
						{
							printf("CMyConfigTable m_iMaxRow:%d != line:%d row:%d\n", m_iMaxRow, line, row + 1);
							return;
						}
					}
					if (line == CMYCONFIGTABLE_TITLE_LINE + 1)
					{
						(*m_pr)[last] = row;
					}
					row = 0;
					pos = line * m_iMaxRow + row;
					last = &((*m_pData)[i + 1]);
					(*m_pv)[pos] = last;
				}
				else if((*m_pData)[i] == '\r')
				{
					(*m_pData)[i] = 0;
				}
				else
				{

				}
			}
		} while (false);
	}
}
CMyConfigTable::~CMyConfigTable()
{
	m_pv->clear();
	delete m_pv;
}
const char* CMyConfigTable::getData(int32_t line, int32_t row) const
{
	int32_t pos = line * m_iMaxRow + row;
	typeV::const_iterator it = m_pv->find(pos);
	if (it == m_pv->end())
	{
		return NULL;
	}
	return it->second;
}
const char* CMyConfigTable::getData(const char* cLine, int32_t row) const
{
	typeLine::const_iterator it_line = m_pl->find(cLine);
	if (it_line == m_pl->end())
	{
		return NULL;
	}
	return getData(it_line->second, row);
}
const char* CMyConfigTable::getData(const char* cLine, const char* cRow) const
{
	typeLine::const_iterator it_line = m_pl->find(cLine);
	if (it_line == m_pl->end())
	{
		return NULL;
	}
	typeRow::const_iterator it_row = m_pr->find(cRow);
	if (it_row == m_pr->end())
	{
		return NULL;
	}

	return getData(it_line->second, it_row->second);
}

