#include "stdafx.h"
#include "util.h"

//���C�h������(WCHAR*)���}���`�o�C�g������(char*)�ɕϊ�
errno_t util::wide2char(LPWSTR wstr, char* cstr) {


	size_t wLen = 0;
	errno_t err = 0;

	//���P�[���w��
	setlocale(LC_ALL, "japanese");
	//�ϊ�
	err = wcstombs_s(&wLen, cstr, 255, wstr, _TRUNCATE);
	return err;
}

std::string util::removeStrRegex(std::string& str, const char* pattern) {
	std::regex re(pattern);
	std::string ret = std::regex_replace(str, re, "");
	return ret;
}


void util::strReplace(std::string& str, const std::string& from, const std::string& to) {
	std::string::size_type pos = 0;
	while (pos = str.find(from, pos), pos != std::string::npos) {
		str.replace(pos, from.length(), to);
		pos += to.length();
	}
}

std::string util::trim(const std::string& string, const char* trimCharacterList)
{
	std::string result;

	// ��������g�������镶���ȊO��������ʒu���������܂��B
	std::string::size_type left = string.find_first_not_of(trimCharacterList);
	if (left != std::string::npos)
	{
		// ��������g�������镶���ȊO�����������ꍇ�́A�����悤�ɉE��������������܂��B
		std::string::size_type right = string.find_last_not_of(trimCharacterList);

		// �߂�l�����肵�܂��B�����ł͉E�����猟�����Ă��A�g�������镶���ȊO���K�����݂���̂Ŕ���s�v�ł��B
		result = string.substr(left, right - left + 1);
	}
	return result;
}

void util::removeChars(std::string& str, const char* chars) {
	for (size_t c = str.find_first_of(chars); c != std::string::npos; c = c = str.find_first_of(chars)) {
		str.erase(c, 1);
	}

}


bool util::create_cmd_process(TCHAR* opt, std::string& out) {
	char mem[80 * 25 * 10];	//	�W���o�͂̃o�b�t�@
	DWORD memSz = 0;	//	�W���o�͂̃o�b�t�@�̗L���f�[�^�[��

	//	�p�C�v�̍쐬
	HANDLE readPipe;
	HANDLE writePipe;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	if (CreatePipe(&readPipe, &writePipe, &sa, 0) == 0) {
		//MessageBox(0, _TEXT("�p�C�v���쐬�ł��܂���ł���"), _TEXT("�G���["), MB_OK);
		return false;
	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdOutput = writePipe;
	si.hStdError = writePipe;
	si.wShowWindow = SW_HIDE;
	TCHAR cmd[MAX_PATH];
	//	�v���Z�X�̋N��(cmd.exe)
	_stprintf_s(cmd, sizeof(cmd) / sizeof(TCHAR), _TEXT("cmd.exe /K %s"), opt);
	if (CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) == 0) {
		//MessageBox(0, _TEXT("�v���Z�X�̍쐬�Ɏ��s���܂���"), _TEXT("�G���["), MB_OK);
		return false;
	}
	HANDLE childProcess = pi.hProcess;
	CloseHandle(pi.hThread);
	char readBuf[1025];
	bool end = false;
	do {
		DWORD totalLen, len;
		if (WaitForSingleObject(childProcess, 100) == WAIT_OBJECT_0)
			end = true;
		if (PeekNamedPipe(readPipe, NULL, 0, NULL, &totalLen, NULL) == 0)
			break;
		if (0 < totalLen) {
			if (ReadFile(readPipe, readBuf, sizeof(readBuf) - 1, &len, NULL) == 0)
				return false;
			readBuf[len] = 0;

			if (sizeof(mem) - 1<memSz + len) {	//�����������ӂ�Ȃ��悤�ɃN���A����
				mem[0] = 0;
				memSz = 0;
			}
			memSz += len;
			strcat_s(mem, sizeof(mem), readBuf);
			if (totalLen>len)	//	�v���Z�X�͏I�����Ă��邪�܂��f�[�^���[���c���Ă���̂ŏI����ۗ�
				end = false;
		}
	} while (end == false);

	if (CloseHandle(writePipe) == 0) {
		//MessageBox(0, _TEXT("�p�C�v�̃N���[�Y�Ɏ��s���܂���"), _TEXT("�G���["), MB_OK);
		return false;
	}
	if (CloseHandle(readPipe) == 0) {
		//MessageBox(0, _TEXT("�p�C�v�̃N���[�Y�Ɏ��s���܂���"), _TEXT("�G���["), MB_OK);
		return false;
	}
	CloseHandle(pi.hProcess);

	out = mem;
	return true;
}

