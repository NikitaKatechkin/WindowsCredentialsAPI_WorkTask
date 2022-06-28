#include <Windows.h>
#include <wincred.h>
#include <iostream>

int main()
{
	const std::wstring login = L"TestAccount_WorkTask";
	std::string password = "randomPassword";

	CREDENTIAL newCred = {};
	newCred.Type = CRED_TYPE_GENERIC;
	newCred.TargetName = const_cast<wchar_t*>(login.c_str());
	newCred.CredentialBlobSize = static_cast<DWORD>(password.length());
	newCred.CredentialBlob = reinterpret_cast<LPBYTE>(const_cast<char*>(password.c_str()));
	newCred.Persist = CRED_PERSIST_SESSION;

	if (CredWrite(&newCred, NULL) == TRUE)
	{
		std::wcout << L"Successfully created new credential." << std::endl;

		PCREDENTIAL credBuffer;

		if (CredRead(login.c_str(), CRED_TYPE_GENERIC, NULL, &credBuffer) == TRUE)
		{
			std::wcout << L"Successfully read credential." << std::endl;

			if ((newCred.CredentialBlobSize == credBuffer->CredentialBlobSize) &&
				(memcmp(newCred.TargetName, credBuffer->TargetName, login.length()) == 0) && 
				(memcmp(reinterpret_cast<char*>(credBuffer->CredentialBlob),
						reinterpret_cast<char*>(newCred.CredentialBlob),
						newCred.CredentialBlobSize) == 0))
			{
				std::wcout << L"Seems like read same thing that were written previously.";
				std::wcout << std::endl;

				std::wcout << L"Written user name: " << newCred.TargetName << std::endl;
				std::wcout << L"Written password: " << reinterpret_cast<char*>(newCred.CredentialBlob);
				std::wcout << std::endl;

				std::wcout << L"Read user name: " << credBuffer->TargetName << std::endl;
				std::wcout << L"Read password: ";
				std::wcout << reinterpret_cast<char*>(credBuffer->CredentialBlob);
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

	return 0;
}