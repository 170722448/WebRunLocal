; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "PluginOK�м��������"
#define MyAppVersion "2.0.5.3"
#define MyAppPublisher "�ɶ�����������޹�˾"
#define MyAppURL "http://www.zorrosoft.com/"
#define MyAppExeName "ZbaSetup.exe"

[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (��Ҫ�����µ� GUID�����ڲ˵��е�� "����|���� GUID"��)
AppId={{9EDC5C47-7F4B-4EAD-AE07-C24938298689}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\PluginOKServer
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
; �������޸�Ϊʵ�ʵ�·����
LicenseFile=D:\Zorro\WebRunLocal\Server\license.txt
; ������ȡ��ע�ͣ����ڷǹ���װģʽ�����У���Ϊ��ǰ�û���װ����
PrivilegesRequired=admin
OutputDir=D:\Zorro\WebRunLocal
OutputBaseFilename=PluginOKServer
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "D:\Zorro\WebRunLocal\Server\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Icons]
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\ZbaSetup.exe";Parameters: "/S /AI";Description: "��װ������";StatusMsg:"���ڰ�װ�����������Ժ�..."

[UninstallRun]
Filename: "{app}\ZbaSetup.exe";Parameters: "/S /AR";StatusMsg:"����ֹͣ��������ж�أ����Ժ�..."

