#define _CRT_SECURE_NO_WARNINGS
#include <fcntl.h> //_O_U16TEXT
#include <io.h>    //_setmode()
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <string.h>

struct THONGTINSINHVIEN  //Khao báo kiểu struct THONGTINSINHVIEN
{
	wchar_t ID[11];
	wchar_t Name[31];
	wchar_t Faculty[31];
	int SchoolYear;
	wchar_t BirthDay[11];
	wchar_t Email[31];
	wchar_t Image[31];
	wchar_t Describe[1001];
	wchar_t Hobby1[101];
	wchar_t Hobby2[101];

	/*Những phát sinh project page*/
	wchar_t NickName[101] = { NULL };
	wchar_t NumberPhone[101] = { NULL };
};

struct HTML  //Khai báo kiểu struct HTML
{
	wchar_t DOCTYPE[196] = L"<!DOCTYPE html PUBLIC \" -//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\">";
	wchar_t html[8] = L"</html>";
	wchar_t Title[8] = L"<title>";
	wchar_t FullName[33] = L"<span class=\"Personal_FullName\">";
	wchar_t Department[34] = L"<div class=\"Personal_Department\">";
	wchar_t Phone[29] = L"<div class=\"Personal_Phone\">";
	wchar_t HinhcanhanKhung[12] = L"<img src=\" ";
	wchar_t TextName[16] = L"<li>Họ và tên: ";
	wchar_t TextID[11] = L"<li>MSSV: ";
	wchar_t TextFaculty[20] = L"<li>Sinh viên khoa ";
	wchar_t TextBirthday[16] = L"<li>Ngày sinh: ";
	wchar_t TextEmail[6] = L"<li> ";
	wchar_t TextHobby1[8] = L"<li>1. ";
	wchar_t TextHobby2[8] = L"<li>2. ";
	wchar_t Description[26] = L"<div class=\"Description\">";
	wchar_t Title1[8] = L"MSSV - ";

	/*Những phát sinh profile page*/
	wchar_t NickName[44] = L"<div class =\"Personal_NickName\">Biệt danh: ";
	wchar_t NumberPhone[20] = L"<li>Số điện thoại: ";
};

int CountLineCSV(FILE);
void InsertSubString(wchar_t*, wchar_t*, int);
void ReadFileCSV(FILE*, THONGTINSINHVIEN *&, int);
void FixHobby2(wchar_t *);
void DeleteTab(wchar_t *);
void CreateFileName(wchar_t *, wchar_t *);
void CreateNickName(wchar_t *, wchar_t *);
void CreateNumberphone(wchar_t *, wchar_t *);
void ReadAndWriteHTML(FILE *, THONGTINSINHVIEN *&, HTML);

/*Đếm số sinh viên có trong File CSV (Tương ứng với số dòng)*/
int CountLineCSV(FILE *&FileIn)
{
	rewind(FileIn);
	int count = 0;
	wchar_t ch;
	while (!feof(FileIn))
	{
		ch = fgetwc(FileIn);
		if (ch == L'\n')
		{
			count++;
		}
	}
	return count;
}

/*Chèn chuổi con vào chuổi cha*/
void InsertSubString(wchar_t *str, wchar_t *substr, int startPos)
{
	int length = wcslen(str);
	int sublength = wcslen(substr);
	wchar_t *temp;
	if (startPos > length)
	{
		startPos = length;
	}
	if (startPos < length)
	{
		temp = new wchar_t[length - startPos + 1];
		wcscpy(temp, str + startPos);
		wcscpy(str + startPos, substr);
		wcscpy(str + startPos + sublength, temp);
		delete[] temp;
	}
	else wcscpy(str + startPos, substr);
}

/*Đọc file CSV*/
void ReadFileCSV(FILE *FileIn, THONGTINSINHVIEN *&SINHVIEN, int CountStudent)  //Đọc FILE CSV
{
	fseek(FileIn, 3L, 0);
	for (int i = 0; i < CountStudent; i++)
	{
		fwscanf(FileIn, L"%[^,],%[^,],%[^,]", SINHVIEN[i].ID, SINHVIEN[i].Name, SINHVIEN[i].Faculty);
		fseek(FileIn, 1L, SEEK_CUR);
		fwscanf(FileIn, L"%d", &SINHVIEN[i].SchoolYear);
		fseek(FileIn, 1L, SEEK_CUR);
		fwscanf(FileIn, L"%[^,],%[^,],%[^,],%[^,]", SINHVIEN[i].BirthDay, SINHVIEN[i].Email, SINHVIEN[i].Image, SINHVIEN[i].Describe);
		fseek(FileIn, 2L, SEEK_CUR);
		fwscanf(FileIn, L"%[^\"]", SINHVIEN[i].Hobby1);
		fseek(FileIn, 3L, SEEK_CUR);
		fgetws(SINHVIEN[i].Hobby2, 101, FileIn);
	}
}

/*Sửa lỗi chuỗi ký tự chứa Hobby2*/
void FixHobby2(wchar_t *HOBBY2)
{
	int length = wcslen(HOBBY2);
	wchar_t xoa[2] = L".";
	*(HOBBY2 + length - 2) = *xoa;
}

/*Xóa các tab trong File*/
void DeleteTab(wchar_t *str)
{
	wchar_t Tab[2] = L"\t";
	int length = wcslen(str);
	for (int i = 0; i < length; i++)
	{
		if (*(str + i) == *(Tab))
		{
			for (int j = i; j < length; j++)
			{
				*(str + j) = *(str + j + 1);
			}
			i--;
		}
	}
}

/*Tạo tên file html cho mỗi sinh viên*/
void CreateFileName(wchar_t *FileName, wchar_t *SINHVIEN)
{
	for (int i = 8; i < 21; i++)
	{
		*(FileName + i) = NULL;
	}
	wchar_t _html[6] = L".html";
	wcscat(FileName, SINHVIEN);
	wcscat(FileName, _html);
}

/*Tạo NickName cho sinh viên*/
void CreateNickName(wchar_t *NickName, wchar_t *TabNickName)
{
	wchar_t div[7] = L"</div>";
	wcscat(NickName, TabNickName);
	wchar_t BietDanh[101];
	wprintf(L"Nhập biệt danh sinh viên:");
	wscanf(L"%ls", BietDanh);
	wcscat(NickName, BietDanh);
	wcscat(NickName, div);
}

/*Tạo SĐT*/
void CreateNumberPhone(wchar_t *NumberPhone, wchar_t *TabNumberPhone)
{
	wchar_t div[6] = L"</li>";
	wcscat(NumberPhone, TabNumberPhone);
	wchar_t SĐT[12];
	wprintf(L"Nhập số điện thoại sinh viên: ");
	wscanf(L"%ls", SĐT);
	wcscat(NumberPhone, SĐT);
	wcscat(NumberPhone, div);
}

/*Đọc thông tin sinh viên và ghi vào file html cho mỗi sinh viên*/
void ReadAndWriteHTML(FILE *FileTemplate, THONGTINSINHVIEN *&SINHVIEN, HTML html, int CountStudent)  //ĐỌC và GHI FILE HTML
{
	wchar_t filename[21] = L"Website/";
	for (int i = 0; i < CountStudent; i++)
	{
		CreateFileName(filename, SINHVIEN[i].ID);
		/*wprintf(L"%ls\n", filename);*/
		FILE *FileOut;
		//FILE* fout = _wfopen(L"userinfo-c.txt", L"w, ccs=UTF-8");
		FileOut = _wfopen(filename, L"w, ccs=UTF-8");
		fwprintf(FileOut, L"%ls \n", html.DOCTYPE);
		wchar_t S[1001];
		rewind(FileTemplate);
		while (!feof(FileTemplate))
		{
			fgetws(S, 1001, FileTemplate);
			DeleteTab(S);
			if (wcsstr(S, html.Title) != NULL)
			{
				int LengthTitle = wcslen(html.Title);
				wchar_t TitleName[101] = L"HCMUS - ";
				wcscat(TitleName, SINHVIEN[i].Name);
				InsertSubString(S, TitleName, LengthTitle);
				fputws(S, FileOut);
				continue;
			}
			if (wcsstr(S, html.FullName) != NULL)
			{
				int LengthFullName = wcslen(html.FullName);
				wchar_t Space[4] = L" - ";
				int LengthSpace = wcslen(Space);
				int length = wcslen(SINHVIEN[i].Name);
				InsertSubString(S, SINHVIEN[i].Name, LengthFullName);
				InsertSubString(S, Space, LengthFullName + length);
				InsertSubString(S, SINHVIEN[i].ID, LengthFullName + length + LengthSpace);
				fputws(S, FileOut);

				/*Chèn thông tin Biệt danh(nếu có)*/
				if (wcslen(SINHVIEN[i].NickName) > 0)
				{
					fputws(SINHVIEN[i].NickName, FileOut);
				}
				continue;
			}
			if (wcsstr(S, html.Department) != NULL)
			{
				int LengthDepartment = wcslen(html.Department);
				_wcsupr(SINHVIEN[i].Faculty);
				InsertSubString(S, SINHVIEN[i].Faculty, LengthDepartment);
				fputws(S, FileOut);
				continue;
			}
			if (wcsstr(S, html.Phone) != NULL)
			{
				int LengthPhone = wcslen(html.Phone);
				InsertSubString(S, SINHVIEN[i].Email, LengthPhone);
				fputws(S, FileOut);
				continue;
			}
			if (wcsstr(S, html.HinhcanhanKhung) != NULL)
			{
				int LengthHinhcanhanKhung = wcslen(html.HinhcanhanKhung);
				wchar_t Image[19] = L"Images/";
				wcscat(Image, SINHVIEN[i].Image);
				InsertSubString(S, Image, LengthHinhcanhanKhung);
				fputws(S, FileOut);
				continue;
			}
			if (wcsstr(S, html.TextName) != NULL)
			{
				int LengthTextName = wcslen(html.TextName);
				InsertSubString(S, SINHVIEN[i].Name, LengthTextName);
				fputws(S, FileOut);
				continue;
			}
			if (wcsstr(S, html.TextID) != NULL)
			{
				int LengthTextID = wcslen(html.TextID);
				InsertSubString(S, SINHVIEN[i].ID, LengthTextID);
				fputws(S, FileOut);
				continue;
			}
			if (wcsstr(S, html.TextFaculty) != NULL)
			{
				int LengthTextFaculty = wcslen(html.TextFaculty);
				_wcslwr(SINHVIEN[i].Faculty);
				InsertSubString(S, SINHVIEN[i].Faculty, LengthTextFaculty);
				fputws(S, FileOut);
				continue;
			}
			if (wcsstr(S, html.TextBirthday) != NULL)
			{
				int LengthTextBirthday = wcslen(html.TextBirthday);
				InsertSubString(S, SINHVIEN[i].BirthDay, LengthTextBirthday);
				fputws(S, FileOut);
				continue;
			}
			if (wcsstr(S, html.TextEmail) != NULL)
			{
				int LengthTextEmail = wcslen(html.TextEmail);
				InsertSubString(S, SINHVIEN[i].Email, LengthTextEmail);
				fputws(S, FileOut);

				/*Chèn thông tin SĐT(nếu có)*/
				if (wcslen(SINHVIEN[i].NumberPhone) > 0)
				{
					fputws(SINHVIEN[i].NumberPhone, FileOut);
				}
				continue;
			}
			if (wcsstr(S, html.TextHobby1) != NULL)
			{
				int LengthTextHobby1 = wcslen(html.TextHobby1);
				InsertSubString(S, SINHVIEN[i].Hobby1, LengthTextHobby1);
				fputws(S, FileOut);
				continue;
			}
			if (wcsstr(S, html.TextHobby2) != NULL)
			{
				int LengthTextHobby2 = wcslen(html.TextHobby2);
				FixHobby2(SINHVIEN[i].Hobby2);
				InsertSubString(S, SINHVIEN[i].Hobby2, LengthTextHobby2);
				fputws(S, FileOut);
				continue;
			}
			if (wcsstr(S, html.Description) != NULL)
			{
				int LengthDescription = wcslen(html.Description);
				InsertSubString(S, SINHVIEN[i].Describe, LengthDescription);
				fputws(S, FileOut);
				continue;
			}
			if (wcsstr(S, html.Title1) != NULL)
			{
				int LengthTitle1 = wcslen(html.Title1);
				wchar_t s[24] = L"Tên sinh viên thực hiện";
				InsertSubString(S, s, LengthTitle1);
				fputws(S, FileOut);
				continue;
			}

			fputws(S, FileOut);
		}
		fwprintf(FileOut, L"%ls \n", html.html);
		fclose(FileOut);
	}

}

int wmain(int argc, wchar_t *argv[])
{
	_setmode(_fileno(stdout), _O_U8TEXT);
	_setmode(_fileno(stdin), _O_U8TEXT);

	HTML html;
	int n = 10;
	FILE *FileIn;
	FileIn = _wfopen(L"FileCSV.csv", L"r, ccs=UTF-8");
	if (!FileIn)
	{
		printf("Khong the tim thay file\n");
		_getch();
		return 0;
	}
	int CountSudent = CountLineCSV(FileIn);
	THONGTINSINHVIEN *SINHVIEN = new THONGTINSINHVIEN[CountSudent];
	ReadFileCSV(FileIn, SINHVIEN, CountSudent);
	fclose(FileIn);

	for (int i = 0; i < CountSudent; i++)  //Xuất thông tin từng sinh viên ra mà hình console
	{
		wprintf(L"Thông tin sinh viên %ld:\n", i + 1);
		wprintf(L"%ls \n %ls \n %ls \n %d \n %ls \n %ls \n %ls \n %ls \n %ls \n %ls\n\n\n", SINHVIEN[i].ID, SINHVIEN[i].Name, SINHVIEN[i].Faculty, SINHVIEN[i].SchoolYear, SINHVIEN[i].BirthDay, SINHVIEN[i].Email, SINHVIEN[i].Image, SINHVIEN[i].Describe, SINHVIEN[i].Hobby1, SINHVIEN[i].Hobby2);
	}

	int choose;
	wprintf(L"Bạn có muốn thêm thông tin biệt danh hay số điện thoại của sinh viên hay không.? Chọn 1 (Đồng ý) or 0(Không đồng ý)\n");
	while (1)
	{
		wscanf(L"%ld", &choose);
		if (choose == 1)
		{
			wchar_t MSSV[11];
			wprintf(L"Nhập mã số sinh viên cần thêm thông tin MSSV: \n");
			wprintf(L"(*Lưu ý: Nhập sai MSSV của sinh viên, khi thêm thông tin sẽ không có giá trị.!!)\n");
			wscanf(L"%ls", &MSSV);
			for (int i = 0; i < CountSudent; i++)
			{
				if (wcsstr(SINHVIEN[i].ID, MSSV) != NULL)
				{
					CreateNickName(SINHVIEN[i].NickName, html.NickName);
					CreateNumberPhone(SINHVIEN[i].NumberPhone, html.NumberPhone); break;
				}
			}
		}
		else
		{
			wprintf(L"Bạn không tiếp tục thêm thông tin sinh viên...!!\n");
			break;
		}
		wprintf(L"\n\n\n");
		wprintf(L"Nhập 1 ==> Tiếp tục thêm thông tin biệt danh hay số điện thoại của sinh viên khác.\n");
		wprintf(L"Nhập 0 ==> Để thoát.\n");
	}
	FILE *FileTemplate;
	FileTemplate = _wfopen(L"Template.html", L"r,ccs=UTF-8");  //Đọc File Template (File mẫu để đọc)
	if (!FileTemplate)
	{
		printf("Khong the tim thay file template\n");
	}
	ReadAndWriteHTML(FileTemplate, SINHVIEN, html, CountSudent);
	fclose(FileTemplate);
	if (SINHVIEN != NULL)
	{
		delete[] SINHVIEN;
	}
	_getch();
	return 0;
}