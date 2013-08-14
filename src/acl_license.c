#include <stdio.h>

// TODO: We should not do this. http://msdn.microsoft.com/en-us/library/5ye5000e%28v=vs.90%29.aspx
#define _T(x) x
typedef char    TCHAR;


#include "TurboActivate.h"
#define TA_GUID _T("438c083e51f294515df5e3.81152356")

void print_ta_error(int hr, const char* str)
{
    switch(hr)
    {
        case TA_FAIL                   : printf("%s (TA_FAIL)\n", str); break;
        case TA_E_PKEY                 : printf("%s (TA_E_PKEY)\n", str); break;
        case TA_E_ACTIVATE             : printf("%s (TA_E_ACTIVATE)\n", str); break;
        case TA_E_INET                 : printf("%s (TA_E_INET)\n", str); break;
        case TA_E_INUSE                : printf("%s (TA_E_INUSE)\n", str); break;
        case TA_E_REVOKED              : printf("%s (TA_E_REVOKED)\n", str); break;
        case TA_E_GUID                 : printf("%s (TA_E_GUID)\n", str); break;
        case TA_E_PDETS                : printf("%s (TA_E_PDETS)\n", str); break;
        case TA_E_TRIAL                : printf("%s (TA_E_TRIAL)\n", str); break;
        case TA_E_TRIAL_EUSED          : printf("%s (TA_E_TRIAL_EUSED)\n", str); break;
        //case TA_E_TRIAL_EEXP           : printf("%s (TA_E_TRIAL_EEXP)\n", str); break;
        case TA_E_EXPIRED              : printf("%s (TA_E_EXPIRED)\n", str); break;
        case TA_E_REACTIVATE           : printf("%s (TA_E_REACTIVATE)\n", str); break;
        case TA_E_COM                  : printf("%s (TA_E_COM)\n", str); break;
        case TA_E_INSUFFICIENT_BUFFER  : printf("%s (TA_E_INSUFFICIENT_BUFFER)\n", str); break;
        case TA_E_PERMISSION           : printf("%s (TA_E_PERMISSION)\n", str); break;
        case TA_E_INVALID_FLAGS        : printf("%s (TA_E_INVALID_FLAGS)\n", str); break;
        case TA_E_IN_VM                : printf("%s (TA_E_IN_VM)\n", str); break;
        case TA_E_EDATA_LONG           : printf("%s (TA_E_EDATA_LONG)\n", str); break;
        case TA_E_INVALID_ARGS         : printf("%s (TA_E_INVALID_ARGS)\n", str); break;
        case TA_E_KEY_FOR_TURBOFLOAT   : printf("%s (TA_E_KEY_FOR_TURBOFLOAT)\n", str); break;
        case TA_E_INET_DELAYED         : printf("%s (TA_E_INET_DELAYED)\n", str); break;
        case TA_E_FEATURES_CHANGED     : printf("%s (TA_E_FEATURES_CHANGED)\n", str); break;
        case TA_E_ANDROID_NOT_INIT     : printf("%s (TA_E_ANDROID_NOT_INIT)\n", str); break;
        default: printf("%s (Unknown: %d)\n", str, hr);
    }
}

int acl_license(const char *key)
{
    HRESULT hr;
    GENUINE_OPTIONS opts;
    opts.nLength = sizeof(GENUINE_OPTIONS);

    // How often to verify with the LimeLM servers (90 days)
    opts.nDaysBetweenChecks = 1;

    // The grace period if TurboActivate couldn't connect to the servers.
    // after the grace period is over IsGenuinEx() will return TA_FAIL instead of
    // TA_E_INET or TA_E_INET_DELAYED
    opts.nGraceDaysOnInetErr = 1;

    // In this example we won't show an error if the activation
    // was done offline by passing the TA_SKIP_OFFLINE flag
    opts.flags = TA_SKIP_OFFLINE;

    //hr = IsGenuineEx(TA_GUID, &opts);
    hr = IsGenuine(TA_GUID);
    if (hr == TA_OK || hr == TA_E_FEATURES_CHANGED || hr == TA_E_INET || hr == TA_E_INET_DELAYED)
    {
        // TCHAR * featureValue;

        print_ta_error(hr, "YourApp is activated and genuine! Enable any app features now.");

        if (hr == TA_E_INET || hr == TA_E_INET_DELAYED)
        {
            // TODO: show a warning to your user that this time (or the last time)
            // the IsGenuineEx() failed to connect to the LimeLM servers.
            print_ta_error(hr, "YourApp is activated, but it failed to verify the activation with the LimeLM servers. You can still use the app for the duration of the grace period.");
        }

        // if this app is activated then you can get a feature value (completely optional)
        // See: http://wyday.com/limelm/help/license-features/
        /*

        hr = GetFeatureValue(_T("your feature value"), 0, 0);

        featureValue = (TCHAR *)malloc(hr * sizeof(TCHAR));

        hr = GetFeatureValue(_T("your feature value"), featureValue, hr);

        if (hr == TA_OK)
        {
#ifdef _WIN32
            wprintf(L"Feature value: %s\n", featureValue);
#else
            printf("Feature value: %s\n", featureValue);
#endif
        }
        else
            printf("Getting feature failed: %d\n", hr);

        free(featureValue);

        */
    }
    else // not activated or genuine
    {
        if(key == NULL)
        {
            uint32_t trialDays = 0;
           	
            // Look in TurboActivate.h for what the error codes mean.
            print_ta_error(hr, "Not activated");

            // Start or re-validate the trial if it has already started.
            // This need to be called at least once before you can use
            // any other trial functions.
            hr = UseTrial(TA_USER);

            if (hr == TA_OK)
            {
                // Get the number of trial days remaining.
                hr = TrialDaysRemaining(TA_GUID, &trialDays);

                if (hr == TA_OK)
                    printf("Trial days remaining: %d\n", trialDays);
                else
                    print_ta_error(hr, "Failed to get the trial days remaining.");
            }
            else
                print_ta_error(hr, "Failed to UseTrial.");
          

        } else {
            printf("Trying to activate key: %s\n", key);
            //TODO: prompt for a product key (if it's not present)
            //Note: here we're just hard-coding the product key to show how you
            //      save the product key and try to activation

            // Also note we're using the TA_SYSTEM flag. This means the activation will be system-wide.
            // However calling using the TA_SYSTEM flag (the first time only) requires system-admin privileges.
            // If your app will never have system admin privileges then you can use the TA_USER flag.
            hr = CheckAndSavePKey(key, TA_USER);
            if (hr == TA_OK)
            {
                printf("Product key saved successfully.\n");

                // try to activate
                hr = Activate();

                if (hr == TA_OK)
                    printf("Activated successfully\n");
                else
                    print_ta_error(hr, "Activation failed.");
            }
            else
            {
                print_ta_error(hr, "Product key failed to save.");
            }
        }
    }

    return hr;
}
