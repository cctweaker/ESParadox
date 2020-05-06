void panel_1_partition_status_names(uint8_t byte, uint8_t bit)
{
    switch (byte)
    {
    case 0:
        switch (bit)
        {
        case 7:
            mesaj = "Pulse Alarm (fire alarm).";
            break;
        case 6:
            mesaj = "Audible alarm.";
            break;
        case 5:
            mesaj = "Silent alarm.";
            break;
        case 4:
            mesaj = "Is / was in Alarm";
            break;
        case 2:
            mesaj = "Stay Arm";
            break;
        case 1:
            mesaj = "Sleep Arm";
            break;
        case 0:
            mesaj = "Arm (Stay/Sleep/Full)";
            break;
        }
        break;
    case 1:
        switch (bit)
        {
        case 7:
            mesaj = "Bell Activated";
            break;
        case 6:
            mesaj = "Auto-Arming Engaged";
            break;
        case 5:
            mesaj = "Recent Closing Delay";
            break;
        case 4:
            mesaj = "Intellizone Delay";
            break;
        case 3:
            mesaj = "At least one zone is bypassed";
            break;
        case 2:
            mesaj = "Alarm(s) is / are in the memory";
            break;
        case 1:
            mesaj = "Entry Delay";
            break;
        case 0:
            mesaj = "Exit Delay";
            break;
        }
        break;
    case 2:
        switch (bit)
        {
        case 7:
            mesaj = "Paramedic Alarm";
            break;
        case 6:
            mesaj = "Reserved";
            break;
        case 5:
            mesaj = "Arm with a Remote";
            break;
        case 4:
            mesaj = "Transmission Delay Finished";
            break;
        case 3:
            mesaj = "Bell Delay Finished";
            break;
        case 2:
            mesaj = "Entry Delay Finished";
            break;
        case 1:
            mesaj = "Exit Delay Finished";
            break;
        case 0:
            mesaj = "Intellizone Delay Finished";
            break;
        }
        break;
    case 3:
        switch (bit)
        {
        case 6:
            mesaj = "Wait windoe (window mode).";
            break;
        case 4:
            mesaj = "In remote delay.";
            break;
        case 2:
            mesaj = "StayD mode active.";
            break;
        case 1:
            mesaj = "Force Arm";
            break;
        case 0:
            mesaj = "Ready Status";
            break;
        }
        break;
    }
}