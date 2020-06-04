

:: Change HKCU to HKLM if you want to install globally.
:: %~dp0 is the directory containing this bat script and ends with a backslash.

REG ADD "HKCU\Software\Google\Chrome\NativeMessagingHosts\com.noteit.native_messaging_host" /ve /t REG_SZ /d "%~dp0manifest.json" /f

%SystemRoot%\Microsoft.NET\Framework64\v4.0.30319\regasm.exe DocProcessor.dll /codebase /u
