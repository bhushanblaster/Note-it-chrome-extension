:: Copyright 2014 The Chromium Authors. All rights reserved.
:: Use of this source code is governed by a BSD-style license that can be
:: found in the LICENSE file.

:: Change HKCU to HKLM if you want to install globally.
:: %~dp0 is the directory containing this bat script and ends with a backslash.

REG ADD "HKCU\Software\Google\Chrome\NativeMessagingHosts\com.dolby.native_messaging_host" /ve /t REG_SZ /d "%~dp0manifest.json" /f

%SystemRoot%\Microsoft.NET\Framework64\v4.0.30319\regasm.exe DocProcessor.dll /codebase /u