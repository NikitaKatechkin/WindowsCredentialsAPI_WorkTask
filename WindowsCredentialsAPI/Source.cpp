#include <Windows.h>
#include <wincred.h>
#include <iostream>

int main()
{
	std::wstring PCName;
	PCName.resize(MAX_COMPUTERNAME_LENGTH + 1);
	DWORD PCNameLength = static_cast<DWORD>(PCName.size());

	if (GetComputerName(&PCName[0], &PCNameLength) == TRUE)
	{
		std::wcout << L"Computer name: " << PCName << std::endl;
		
		wchar_t password[] = L"randomPassword";
		DWORD blobSize = lstrlen(password);

		CREDENTIAL newCred = {};
		newCred.Type = CRED_TYPE_GENERIC;
		newCred.TargetName = const_cast<wchar_t*>(L"TestCred_WorkTask");
		newCred.CredentialBlobSize = blobSize;
		newCred.CredentialBlob = reinterpret_cast<LPBYTE>(password);
		newCred.Persist = CRED_PERSIST_SESSION;

		if (CredWrite(&newCred, NULL) == TRUE)
		{
			std::wcout << L"Successfully created new credential." << std::endl;

			PCREDENTIAL credBuffer = {};

			if (CredRead(const_cast<wchar_t*>(L"TestCred_WorkTask"), 
				CRED_TYPE_GENERIC, 
				NULL, 
				&credBuffer) == TRUE)
			{
				std::wcout << L"Successfully read credential." << std::endl;

				if ((newCred.CredentialBlobSize == credBuffer->CredentialBlobSize) && 
					(memcmp(newCred.TargetName, credBuffer->TargetName, lstrlen(L"TestCred_WorkTask")) == 0))
				{
					std::wcout << L"Seems like read same thing that were written previously.";
					std::wcout << std::endl;
				}
				else
				{
					std::wcout << L"Seems like read thing different from";
					std::wcout << " that were written previously.";
					std::wcout << std::endl;
				}
			}
			else
			{
				std::wcout << L"Failed to read credentials with GLE = ";
				std::wcout << GetLastError() << std::endl;
			}
		}
		else
		{
			std::wcout << L"Failed to write credentials with GLE = " << GetLastError() << std::endl;
		}
	}
	else
	{
		std::wcout << L"Failed to get computer name with GLE = " << GetLastError() << std::endl;
	}

	return 0;
}