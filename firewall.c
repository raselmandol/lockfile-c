#include <stdio.h>
#include <windows.h>
#include <netfw.h>

//Linking with 'ole32.lib' and 'oleaut32.lib'

int main() {
    HRESULT hr;
    INetFwProfile* fwProfile = NULL;
    INetFwRules* fwRules = NULL;
    INetFwRule* newRule = NULL;
    BSTR bstrRuleName = SysAllocString(L"BlockOutgoingRule");
    hr = CoInitializeEx(0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        wprintf(L"CoInitializeEx failed: 0x%x\n", hr);
        return 1;
    }
    hr = CoCreateInstance(__uuidof(NetFwPolicy2), NULL, CLSCTX_INPROC_SERVER, __uuidof(INetFwProfile), (void**)&fwProfile);
    if (FAILED(hr)) {
        wprintf(L"CoCreateInstance failed: 0x%x\n", hr);
        CoUninitialize();
        return 1;
    }
    
    hr = INetFwProfile_get_Rules(fwProfile, &fwRules);
    if (FAILED(hr)) {
        wprintf(L"INetFwProfile_get_Rules failed: 0x%x\n", hr);
        fwProfile->lpVtbl->Release(fwProfile);
        CoUninitialize();
        return 1;
    }
    hr = CoCreateInstance(__uuidof(NetFwRule), NULL, CLSCTX_INPROC_SERVER, __uuidof(INetFwRule), (void**)&newRule);
    if (FAILED(hr)) {
        wprintf(L"CoCreateInstance failed: 0x%x\n", hr);
        fwRules->lpVtbl->Release(fwRules);
        fwProfile->lpVtbl->Release(fwProfile);
        CoUninitialize();
        return 1;
    }
    INetFwRule_put_Action(newRule, NET_FW_ACTION_BLOCK);
    INetFwRule_put_Direction(newRule, NET_FW_RULE_DIR_OUT);
    INetFwRule_put_Name(newRule, bstrRuleName);
    INetFwRule_put_ApplicationName(newRule, L"%SystemRoot%\\system32\\svchost.exe");  // Example application path
    hr = INetFwRules_Add(fwRules, newRule);
    if (FAILED(hr)) {
        wprintf(L"INetFwRules_Add failed: 0x%x\n", hr);
        newRule->lpVtbl->Release(newRule);
        fwRules->lpVtbl->Release(fwRules);
        fwProfile->lpVtbl->Release(fwProfile);
        CoUninitialize();
        return 1;
    }
    wprintf(L"Outgoing connections blocked.\n");
    newRule->lpVtbl->Release(newRule);
    fwRules->lpVtbl->Release(fwRules);
    fwProfile->lpVtbl->Release(fwProfile);
    CoUninitialize();
    return 0;
}
