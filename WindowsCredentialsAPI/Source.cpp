#include <Windows.h>
#include <wincred.h>
#include <iostream>

/**
int main()
{
	std::wstring PCName;
	PCName.resize(MAX_COMPUTERNAME_LENGTH + 1);
	DWORD PCNameLength = static_cast<DWORD>(PCName.size());

	if (GetComputerName(&PCName[0], &PCNameLength) == TRUE)
	{
		std::wcout << L"Computer name: " << PCName << std::endl;
		
		//wchar_t password[] = L"randomPassword";
		//DWORD blobSize = lstrlen(password);

		wchar_t credName[] = L"TestCred_WorkTask";

		std::wstring password = L"random";

		CREDENTIAL newCred = {};
		newCred.Type = CRED_TYPE_GENERIC;
		newCred.TargetName = (credName);
		newCred.CredentialBlobSize = static_cast<DWORD>(password.length());
		newCred.CredentialBlob = reinterpret_cast<LPBYTE>(const_cast<wchar_t*>(password.c_str()));
		newCred.Persist = CRED_PERSIST_SESSION;

		if (CredWrite(&newCred, NULL) == TRUE)
		{
			std::wcout << L"Successfully created new credential." << std::endl;

			PCREDENTIAL credBuffer;

			if (CredRead(credName, CRED_TYPE_GENERIC, NULL, &credBuffer) == TRUE)
			{
				std::wcout << L"Successfully read credential." << std::endl;

				if ((newCred.CredentialBlobSize == credBuffer->CredentialBlobSize) && 
					(memcmp(newCred.TargetName, credBuffer->TargetName, lstrlen(credName)) == 0))
				{
					std::wcout << L"Seems like read same thing that were written previously.";
					std::wcout << std::endl;

					std::wcout << "User name: " << newCred.TargetName << std::endl;
					std::wcout << "Password: " << reinterpret_cast<wchar_t*>(newCred.CredentialBlob);
					std::wcout << std::endl;

					wchar_t* pswdBuffer = new wchar_t[credBuffer->CredentialBlobSize];
					memcpy_s(pswdBuffer, credBuffer->CredentialBlobSize, 
							 credBuffer->CredentialBlob, credBuffer->CredentialBlobSize);

					std::wcout << "User name: " << credBuffer->TargetName << std::endl;
					std::wcout << "Password: ";
					std::wcout << pswdBuffer;
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
**/

int main()
{
	const wchar_t login[] = L"TestAccount_WorkTask";

	char password[] = "randomPassword";
	DWORD passwordSize = static_cast<DWORD>(strlen(password) + 1);

	CREDENTIAL newCred = {};
	newCred.Type = CRED_TYPE_GENERIC;
	newCred.TargetName = const_cast<wchar_t*>(login);
	newCred.CredentialBlobSize = passwordSize;
	newCred.CredentialBlob = reinterpret_cast<LPBYTE>(password);
	newCred.Persist = CRED_PERSIST_SESSION;

	if (CredWrite(&newCred, NULL) == TRUE)
	{
		std::wcout << L"Successfully created new credential." << std::endl;

		PCREDENTIAL credBuffer;

		if (CredRead(login, CRED_TYPE_GENERIC, NULL, &credBuffer) == TRUE)
		{
			std::wcout << L"Successfully read credential." << std::endl;

			if ((newCred.CredentialBlobSize == credBuffer->CredentialBlobSize) &&
				(memcmp(newCred.TargetName, credBuffer->TargetName, lstrlen(login)) == 0) && 
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