#ifndef __MYFILE_H__
#define __MYFILE_H__

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

//��ȡ���������ļ�  vec����
class FRAMEWORK_TOOL_API CMyConfigVec
{
public:
    using typeV = std::vector<std::shared_ptr<std::string>>;
public:
    CMyConfigVec(const char* file_name);
    ~CMyConfigVec();

    const typeV& getConfigVec()const { return *m_pv; }
private:
    CMyConfigVec() {}
    typeV*           m_pv = NULL;
};

//��ȡ k=v map����
class FRAMEWORK_TOOL_API CMyConfigKv
{
public:
    using typeV = std::map<std::string,std::string>;
public:
    CMyConfigKv(const char* file_name);
    ~CMyConfigKv();

    const typeV& getConfigKv()const { return *m_pv; }
    const char* getConfigKv(const char* k)const;
private:
    CMyConfigKv() {}
    typeV* m_pv = NULL;

};
#define CMyConfigKvInt(conf,k,v)                            \
if(const char* pc = conf.getConfigKv(k))                    \
{                                                           \
    v = atoi(pc);                                           \
}
#define CMyConfigKvChar(conf,k,v)                           \
if(const char* pc = conf.getConfigKv(k))                    \
{                                                           \
    v = pc;                                                 \
}


//��ȡexcel��  
class FRAMEWORK_TOOL_API CMyConfigTable
{
public:
    using typeV = std::map<int32_t, const char*>;
    using typeRow = std::map<std::string, int32_t>;
    using typeLine = std::map<std::string, int32_t>;
public:
    CMyConfigTable(const char* file_name);
    ~CMyConfigTable();

    int32_t getMaxRow() const{ return m_iMaxRow; }//�����
    int32_t getMaxLine() const { return m_iMaxLine; }//�����
    const char* getData(int32_t line = 2, int32_t row = 0) const;//��ȡ�ڼ��� �ڼ�������
    const char* getData(const char* cLine, int32_t row = 0) const;//��ȡ "index" �����У� n ������
    const char* getData(const char* cLine, const char* cRow) const;//��ȡ "index" �����У� title ������

private:
    CMyConfigTable() {}
    std::string *   m_pData = NULL;
    int32_t         m_iMaxRow;//�����
    int32_t         m_iMaxLine;//�����
    typeV* m_pv = NULL;
    typeRow* m_pr = NULL;
    typeLine* m_pl = NULL;
};








#endif //__MYFILE_H__
