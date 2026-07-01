#include "custom_sender.h"

#include "bms-utils.h"
#include "can-networks.h"
#include "inverters-utils.h"
#include "primary-utils.h"

#define bms_message_name_from_id(a, b)       sprintf(b, "Bms dummy\n")
#define inverters_message_name_from_id(a, b) sprintf(b, "Inverters dummy%s\n")

#define bms_to_string_from_id(a, b, c)       ((void)0)
#define inverters_to_string_from_id(a, b, c) ((void)0)

int primary_message_name_from_id(uint16_t id, char *buffer) {
    switch (id) {
        case CAN_PRIMARY_MESSAGE_FRAME_ID_TELEMETRY_VERSION:
            return sprintf(buffer, "%s", "telemetry_version");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_TELEMETRY_STATUS:
            return sprintf(buffer, "%s", "telemetry_status");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_ECU_VERSION:
            return sprintf(buffer, "%s", "ecu_version");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_ECU_STATUS:
            return sprintf(buffer, "%s", "ecu_status");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_ECU_SET_HV_BMS_STATUS:
            return sprintf(buffer, "%s", "ecu_set_hv_bms_status");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_PEDALS_VERSION:
            return sprintf(buffer, "%s", "pedals_version");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_PEDALS_STATUS:
            return sprintf(buffer, "%s", "pedals_status");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_PEDALS_APPS:
            return sprintf(buffer, "%s", "pedals_apps");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_PEDALS_THROTTLE:
            return sprintf(buffer, "%s", "pedals_throttle");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_PEDALS_BRAKE:
            return sprintf(buffer, "%s", "pedals_brake");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_STATUS:
            return sprintf(buffer, "%s", "hv_bms_status");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_VERSION:
            return sprintf(buffer, "%s", "hv_bms_version");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_VERSION:
            return sprintf(buffer, "%s", "hv_bms_cellboard_version");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_FLASH:
            return sprintf(buffer, "%s", "hv_bms_flash");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_MAINBOARD_TX:
            return sprintf(buffer, "%s", "hv_bms_mainboard_tx");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_MAINBOARD_RX:
            return sprintf(buffer, "%s", "hv_bms_mainboard_rx");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_TX_0:
            return sprintf(buffer, "%s", "hv_bms_cellboard_openblt_tx_0");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_RX_0:
            return sprintf(buffer, "%s", "hv_bms_cellboard_openblt_rx_0");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_TX_1:
            return sprintf(buffer, "%s", "hv_bms_cellboard_openblt_tx_1");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_RX_1:
            return sprintf(buffer, "%s", "hv_bms_cellboard_openblt_rx_1");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_TX_2:
            return sprintf(buffer, "%s", "hv_bms_cellboard_openblt_tx_2");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_RX_2:
            return sprintf(buffer, "%s", "hv_bms_cellboard_openblt_rx_2");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_TX_3:
            return sprintf(buffer, "%s", "hv_bms_cellboard_openblt_tx_3");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_RX_3:
            return sprintf(buffer, "%s", "hv_bms_cellboard_openblt_rx_3");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_TX_4:
            return sprintf(buffer, "%s", "hv_bms_cellboard_openblt_tx_4");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_RX_4:
            return sprintf(buffer, "%s", "hv_bms_cellboard_openblt_rx_4");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_TX_5:
            return sprintf(buffer, "%s", "hv_bms_cellboard_openblt_tx_5");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_RX_5:
            return sprintf(buffer, "%s", "hv_bms_cellboard_openblt_rx_5");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_BALANCING_STATUS:
            return sprintf(buffer, "%s", "hv_bms_balancing_status");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_TS_VOLTAGE:
            return sprintf(buffer, "%s", "hv_bms_ts_voltage");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_VOLTAGES:
            return sprintf(buffer, "%s", "hv_bms_cellboard_voltages");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_VOLTAGES_INFO:
            return sprintf(buffer, "%s", "hv_bms_cellboard_voltages_info");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_TEMPERATURES:
            return sprintf(buffer, "%s", "hv_bms_cellboard_temperatures");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_TEMPERATURES_INFO:
            return sprintf(buffer, "%s", "hv_bms_cellboard_temperatures_info");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CURRENT:
            return sprintf(buffer, "%s", "hv_bms_current");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_POWER:
            return sprintf(buffer, "%s", "hv_bms_power");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_IMD:
            return sprintf(buffer, "%s", "hv_bms_imd");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_FEEDBACK_STATUS:
            return sprintf(buffer, "%s", "hv_bms_feedback_status");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_FEEDBACK_TS_VOLTAGE:
            return sprintf(buffer, "%s", "hv_bms_feedback_ts_voltage");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_FEEDBACK_SD_VOLTAGE:
            return sprintf(buffer, "%s", "hv_bms_feedback_sd_voltage");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_FEEDBACK_MISC_VOLTAGE:
            return sprintf(buffer, "%s", "hv_bms_feedback_misc_voltage");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_ERRORS:
            return sprintf(buffer, "%s", "hv_bms_errors");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_STEERING_WHEEL_FLASH:
            return sprintf(buffer, "%s", "steering_wheel_flash");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_STEERING_WHEEL_OPENBLT_TX:
            return sprintf(buffer, "%s", "steering_wheel_openblt_tx");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_STEERING_WHEEL_OPENBLT_RX:
            return sprintf(buffer, "%s", "steering_wheel_openblt_rx");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_STEERING_WHEEL_VERSION:
            return sprintf(buffer, "%s", "steering_wheel_version");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_STEERING_WHEEL_SET_ECU_STATUS:
            return sprintf(buffer, "%s", "steering_wheel_set_ecu_status");
        case CAN_PRIMARY_MESSAGE_FRAME_ID_STEERING_WHEEL_SET_HV_BMS_BALANCING_STATUS:
            return sprintf(buffer, "%s", "steering_wheel_set_hv_bms_balancing_status");
        default:
            return 0;
    }
}

int primary_to_string_from_id(uint16_t message_id, void *message, char *buffer) {
    switch (message_id) {
        case CAN_PRIMARY_MESSAGE_FRAME_ID_TELEMETRY_VERSION:
            return sprintf(
                buffer,
                "%" PRIu32 ","
                "%" PRIu32 "",
                ((struct CanPrimaryTelemetryVersion *)message)->buildtime_s,
                ((struct CanPrimaryTelemetryVersion *)message)->canlibbuildtime_s);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_TELEMETRY_STATUS:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryTelemetryStatus *)message)->name);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_ECU_VERSION:
            return sprintf(
                buffer,
                "%" PRIu32 ","
                "%" PRIu32 "",
                ((struct CanPrimaryEcuVersion *)message)->buildtime_s,
                ((struct CanPrimaryEcuVersion *)message)->canlibbuildtime_s);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_ECU_STATUS:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryEcuStatus *)message)->name);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_ECU_SET_HV_BMS_STATUS:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryEcuSetHvBmsStatus *)message)->targetstatus);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_PEDALS_VERSION:
            return sprintf(
                buffer,
                "%" PRIu32 ","
                "%" PRIu32 "",
                ((struct CanPrimaryPedalsVersion *)message)->buildtime_s,
                ((struct CanPrimaryPedalsVersion *)message)->canlibbuildtime_s);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_PEDALS_STATUS:
            return sprintf(buffer, "%" PRIu8 ",", ((struct CanPrimaryPedalsStatus *)message)->name);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_PEDALS_APPS:
            return sprintf(
                buffer,
                "%f,%f,%f",
                ((struct CanPrimaryPedalsApps *)message)->travelfirst_pct,
                ((struct CanPrimaryPedalsApps *)message)->travelsecond_pct,
                ((struct CanPrimaryPedalsApps *)message)->travelthird_pct);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_PEDALS_THROTTLE:
            return sprintf(
                buffer,
                "%f,"
                "%" PRIu8 ",",
                ((struct CanPrimaryPedalsThrottle *)message)->travel_pct,
                ((struct CanPrimaryPedalsThrottle *)message)->status);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_PEDALS_BRAKE:
            return sprintf(
                buffer,
                "%f,%f,%f",
                ((struct CanPrimaryPedalsBrake *)message)->travel_pct,
                ((struct CanPrimaryPedalsBrake *)message)->pressurefront_bar,
                ((struct CanPrimaryPedalsBrake *)message)->pressurerear_bar);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_STATUS:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsStatus *)message)->name);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_VERSION:
            return sprintf(
                buffer,
                "%" PRIu32 ","
                "%" PRIu32 "",
                ((struct CanPrimaryHvBmsVersion *)message)->buildtime_s,
                ((struct CanPrimaryHvBmsVersion *)message)->canlibbuildtime_s);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_VERSION:
            return sprintf(
                buffer,
                "%" PRIu8 ","
                "%" PRIu32 ","
                "%" PRIu32 "",
                ((struct CanPrimaryHvBmsCellboardVersion *)message)->id,
                ((struct CanPrimaryHvBmsCellboardVersion *)message)->buildtime_s,
                ((struct CanPrimaryHvBmsCellboardVersion *)message)->canlibbuildtime_s);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_FLASH:
            return sprintf(
                buffer,
                "%" PRIu8 ","
                "%" PRIu8 "",
                ((struct CanPrimaryHvBmsFlash *)message)->forward_bool,
                ((struct CanPrimaryHvBmsFlash *)message)->cellboardid);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_MAINBOARD_TX:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsMainboardTx *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_MAINBOARD_RX:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsMainboardRx *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_TX_0:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsCellboardOpenbltTx0 *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_RX_0:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsCellboardOpenbltRx0 *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_TX_1:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsCellboardOpenbltTx1 *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_RX_1:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsCellboardOpenbltRx1 *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_TX_2:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsCellboardOpenbltTx2 *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_RX_2:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsCellboardOpenbltRx2 *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_TX_3:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsCellboardOpenbltTx3 *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_RX_3:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsCellboardOpenbltRx3 *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_TX_4:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsCellboardOpenbltTx4 *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_RX_4:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsCellboardOpenbltRx4 *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_TX_5:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsCellboardOpenbltTx5 *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_OPENBLT_RX_5:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimaryHvBmsCellboardOpenbltRx5 *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_BALANCING_STATUS:
            return sprintf(
                buffer,
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 "",
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->cellboardid,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancing_bool,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->errorcancomm,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->errorltccomm,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->erroropenwire,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->errortempcomm0,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->errortempcomm1,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->errortempcomm2,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->errortempcomm3,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->errortempcomm4,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->errortempcomm5,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell0,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell1,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell2,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell3,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell4,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell5,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell6,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell7,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell8,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell9,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell10,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell11,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell12,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell13,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell14,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell15,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell16,
                ((struct CanPrimaryHvBmsBalancingStatus *)message)->balancingcell17);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_TS_VOLTAGE:
            return sprintf(
                buffer,
                "%f,%f,%f",
                ((struct CanPrimaryHvBmsTsVoltage *)message)->pack_v,
                ((struct CanPrimaryHvBmsTsVoltage *)message)->cellsum_v,
                ((struct CanPrimaryHvBmsTsVoltage *)message)->bus_v);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_VOLTAGES:
            return sprintf(
                buffer,
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%f,%f,%f",
                ((struct CanPrimaryHvBmsCellboardVoltages *)message)->cellboardid,
                ((struct CanPrimaryHvBmsCellboardVoltages *)message)->startindex,
                ((struct CanPrimaryHvBmsCellboardVoltages *)message)->first_v,
                ((struct CanPrimaryHvBmsCellboardVoltages *)message)->second_v,
                ((struct CanPrimaryHvBmsCellboardVoltages *)message)->third_v);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_VOLTAGES_INFO:
            return sprintf(
                buffer,
                "%f,%f,%f,%f",
                ((struct CanPrimaryHvBmsCellboardVoltagesInfo *)message)->max_v,
                ((struct CanPrimaryHvBmsCellboardVoltagesInfo *)message)->min_v,
                ((struct CanPrimaryHvBmsCellboardVoltagesInfo *)message)->delta_v,
                ((struct CanPrimaryHvBmsCellboardVoltagesInfo *)message)->average_v);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_TEMPERATURES:
            return sprintf(
                buffer,
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%f,%f,%f,%f",
                ((struct CanPrimaryHvBmsCellboardTemperatures *)message)->cellboardid,
                ((struct CanPrimaryHvBmsCellboardTemperatures *)message)->startindex,
                ((struct CanPrimaryHvBmsCellboardTemperatures *)message)->first_c,
                ((struct CanPrimaryHvBmsCellboardTemperatures *)message)->second_c,
                ((struct CanPrimaryHvBmsCellboardTemperatures *)message)->third_c,
                ((struct CanPrimaryHvBmsCellboardTemperatures *)message)->fourth_c);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CELLBOARD_TEMPERATURES_INFO:
            return sprintf(
                buffer,
                "%f,%f,%f",
                ((struct CanPrimaryHvBmsCellboardTemperaturesInfo *)message)->max_c,
                ((struct CanPrimaryHvBmsCellboardTemperaturesInfo *)message)->min_c,
                ((struct CanPrimaryHvBmsCellboardTemperaturesInfo *)message)->average_c);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_CURRENT:
            return sprintf(buffer, "%f", ((struct CanPrimaryHvBmsCurrent *)message)->current_a);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_POWER:
            return sprintf(buffer, "%f", ((struct CanPrimaryHvBmsPower *)message)->power_kw);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_IMD:
            return sprintf(
                buffer,
                "%" PRIu8 ","
                "%f,%f,"
                "%" PRIu16 ","
                "%" PRIu8 ","
                "%" PRIu8 "",
                ((struct CanPrimaryHvBmsImd *)message)->status,
                ((struct CanPrimaryHvBmsImd *)message)->frequency_hz,
                ((struct CanPrimaryHvBmsImd *)message)->dutycycle_pct,
                ((struct CanPrimaryHvBmsImd *)message)->details,
                ((struct CanPrimaryHvBmsImd *)message)->fault_bool,
                ((struct CanPrimaryHvBmsImd *)message)->period_ms);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_FEEDBACK_STATUS:
            return sprintf(
                buffer,
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 "",
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->implausibility,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->imdcockpit,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->tsalgreenfaultlatched,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->bmscockpit,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->extlatched,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->tsalgreen,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->tsover60v,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->airnstatus,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->airpstatus,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->airpgate,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->airngate,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->prechargestatus,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->tspover60v,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->imdfault,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->checkmux,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->sdend,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->sdout,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->sdin,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->sdbms,
                ((struct CanPrimaryHvBmsFeedbackStatus *)message)->sdimd);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_FEEDBACK_TS_VOLTAGE:
            return sprintf(
                buffer,
                "%f,%f,%f,%f,%f,%f,%f",
                ((struct CanPrimaryHvBmsFeedbackTsVoltage *)message)->airnstatus_v,
                ((struct CanPrimaryHvBmsFeedbackTsVoltage *)message)->airngate_v,
                ((struct CanPrimaryHvBmsFeedbackTsVoltage *)message)->prechargestatus_v,
                ((struct CanPrimaryHvBmsFeedbackTsVoltage *)message)->airpstatus_v,
                ((struct CanPrimaryHvBmsFeedbackTsVoltage *)message)->airpgate_v,
                ((struct CanPrimaryHvBmsFeedbackTsVoltage *)message)->tsover60v_v,
                ((struct CanPrimaryHvBmsFeedbackTsVoltage *)message)->tspover60v_v);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_FEEDBACK_SD_VOLTAGE:
            return sprintf(
                buffer,
                "%f,%f,%f,%f,%f",
                ((struct CanPrimaryHvBmsFeedbackSdVoltage *)message)->sdend_v,
                ((struct CanPrimaryHvBmsFeedbackSdVoltage *)message)->sdout_v,
                ((struct CanPrimaryHvBmsFeedbackSdVoltage *)message)->sdin_v,
                ((struct CanPrimaryHvBmsFeedbackSdVoltage *)message)->sdbms_v,
                ((struct CanPrimaryHvBmsFeedbackSdVoltage *)message)->sdimd_v);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_FEEDBACK_MISC_VOLTAGE:
            return sprintf(
                buffer,
                "%f,%f,%f,%f,%f,%f,%f",
                ((struct CanPrimaryHvBmsFeedbackMiscVoltage *)message)->implausibility_v,
                ((struct CanPrimaryHvBmsFeedbackMiscVoltage *)message)->imdcockpit_v,
                ((struct CanPrimaryHvBmsFeedbackMiscVoltage *)message)->tsalgreenfaultlatched_v,
                ((struct CanPrimaryHvBmsFeedbackMiscVoltage *)message)->bmscockpit_v,
                ((struct CanPrimaryHvBmsFeedbackMiscVoltage *)message)->extlatched_v,
                ((struct CanPrimaryHvBmsFeedbackMiscVoltage *)message)->tsalgreen_v,
                ((struct CanPrimaryHvBmsFeedbackMiscVoltage *)message)->imdfault_v,
                ((struct CanPrimaryHvBmsFeedbackMiscVoltage *)message)->checkmux_v);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_HV_BMS_ERRORS:
            return sprintf(
                buffer,
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 ","
                "%" PRIu8 "",
                ((struct CanPrimaryHvBmsErrors *)message)->cellundervoltage,
                ((struct CanPrimaryHvBmsErrors *)message)->cellovervoltage,
                ((struct CanPrimaryHvBmsErrors *)message)->cellundertemperature,
                ((struct CanPrimaryHvBmsErrors *)message)->cellovertemperature,
                ((struct CanPrimaryHvBmsErrors *)message)->overcurrent,
                ((struct CanPrimaryHvBmsErrors *)message)->cancommunication,
                ((struct CanPrimaryHvBmsErrors *)message)->tsvoltagemismatch,
                ((struct CanPrimaryHvBmsErrors *)message)->cellboardcommunication,
                ((struct CanPrimaryHvBmsErrors *)message)->cellboardinternal,
                ((struct CanPrimaryHvBmsErrors *)message)->connectordisconnected,
                ((struct CanPrimaryHvBmsErrors *)message)->fansdisconnected,
                ((struct CanPrimaryHvBmsErrors *)message)->feedback,
                ((struct CanPrimaryHvBmsErrors *)message)->feedbackcircuitry,
                ((struct CanPrimaryHvBmsErrors *)message)->eepromcommunication,
                ((struct CanPrimaryHvBmsErrors *)message)->eepromwrite);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_STEERING_WHEEL_FLASH:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimarySteeringWheelFlash *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_STEERING_WHEEL_OPENBLT_TX:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimarySteeringWheelOpenbltTx *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_STEERING_WHEEL_OPENBLT_RX:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimarySteeringWheelOpenbltRx *)message)->dummy);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_STEERING_WHEEL_VERSION:
            return sprintf(
                buffer,
                "%" PRIu32 ","
                "%" PRIu32 "",
                ((struct CanPrimarySteeringWheelVersion *)message)->buildtime_s,
                ((struct CanPrimarySteeringWheelVersion *)message)->canlibbuildtime_s);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_STEERING_WHEEL_SET_ECU_STATUS:
            return sprintf(buffer, "%" PRIu8 "", ((struct CanPrimarySteeringWheelSetEcuStatus *)message)->targetstatus);
        case CAN_PRIMARY_MESSAGE_FRAME_ID_STEERING_WHEEL_SET_HV_BMS_BALANCING_STATUS:
            return sprintf(
                buffer,
                "%" PRIu8 ","
                "%" PRIu16 "",
                ((struct CanPrimarySteeringWheelSetHvBmsBalancingStatus *)message)->targetstatus,
                ((struct CanPrimarySteeringWheelSetHvBmsBalancingStatus *)message)->threshold);
        default:
            return -1;
    }
}

struct sockaddr_can address                  = {0};
int can_sockets[N_CAN_DEVICES]               = {0};
char can_devices[N_CAN_DEVICES][GEN_STR_LEN] = CAN_DEVICES;

struct pollfd poll_fds[N_CAN_DEVICES + 1];
struct can_frame candump_data;
can_message_log_t msg_log[CAN_PRIMARY_MESSAGE_COUNT + CAN_INVERTERS_MESSAGE_COUNT + CAN_BMS_MESSAGE_COUNT];
struct can_frame can_selected_msg;
int msg_log_count;

union CanNetworkMessage can_network_message;

int idx_offset;

int open_can_socket(const char *device) {
    struct ifreq ifr;
    int can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (can_socket < 0) {
        return -1;
    }
    strcpy(ifr.ifr_name, device);
    ioctl(can_socket, SIOCGIFINDEX, &ifr);
    address.can_family                    = AF_CAN;
    address.can_ifindex                   = ifr.ifr_ifindex;
    const struct sockaddr *sockaddr_param = (struct sockaddr *)&(address);
    if (bind(can_socket, sockaddr_param, sizeof(address)) < 0) {
        return -2;
    }
    return can_socket;
}

bool close_socket(int can_socket) {
    if (close(can_socket) < 0) {
        fatal_error("Error closing can socket");
        return false;
    }
    return true;
}

int can_send(can_message_t *msg, int can_socket) {
    if (msg->size < 0 || msg->size > 8)
        return -1;
    struct can_frame frame;
    frame.can_id  = msg->id;
    frame.can_dlc = msg->size;
    memcpy(frame.data, msg->data, msg->size * sizeof(char));
    return write(can_socket, &frame, sizeof(frame));
}

int _scrwidth                                  = 0;
int _scrheight                                 = 0;
enum application_tabs_t ctab                   = main_menu;
int chosen_msg_idx                             = -1;
enum interfaces_t chosen_interface             = -1;
char current_fields[MAX_N_FIELDS][GEN_STR_LEN] = {0};
int current_n_fields                           = -1;
bool _is_focused                               = false;

extern int n_shown_msgs;
extern enum interfaces_t chosen_intf;

can_msg_metadata_t metadata_msgs[CAN_PRIMARY_MESSAGE_COUNT + CAN_INVERTERS_MESSAGE_COUNT + CAN_BMS_MESSAGE_COUNT];

void fatal_error(const char *error_msg) {
    exit(1);
}

int get_intf_tot_msg() {
    switch (chosen_intf) {
        case primary_intf:
            return CAN_PRIMARY_MESSAGE_COUNT;
        case inverter_intf:
            return CAN_INVERTERS_MESSAGE_COUNT;
        case bms_intf:
            return CAN_BMS_MESSAGE_COUNT;
        default:
            return 0;
    }
}

int get_intf_base_idx() {
    switch (chosen_intf) {
        case primary_intf:
            return 0;
        case inverter_intf:
            return CAN_PRIMARY_MESSAGE_COUNT;
        case bms_intf:
            return CAN_PRIMARY_MESSAGE_COUNT + CAN_INVERTERS_MESSAGE_COUNT;
        default:
            return 0;
    }
}

int get_idx_from_intf(int bidx, enum interfaces_t intf) {
    switch (intf) {
        case primary_intf:
            return bidx;
        case inverter_intf:
            return bidx + CAN_PRIMARY_MESSAGE_COUNT;
        case bms_intf:
            return bidx + CAN_PRIMARY_MESSAGE_COUNT + CAN_INVERTERS_MESSAGE_COUNT;
        default:
            return bidx;
    }
}

int init_canlib_metadata(void) {
    for (size_t iidx = 0; iidx < CAN_PRIMARY_MESSAGE_COUNT; iidx++) {
        uint16_t id = can_primary_api_id_from_index(iidx);
        if (id == 0xFF) {
            fatal_error("Could not retrieve primary message id from index");
        }
        int imsg                     = get_idx_from_intf(iidx, primary_intf);
        int n_field                  = primary_n_fields_from_id(id);
        metadata_msgs[imsg].n_fields = n_field;
        metadata_msgs[imsg].msg_name = (char *)malloc(GEN_STR_LEN * sizeof(char));
        if (!primary_message_name_from_id(id, metadata_msgs[imsg].msg_name)) {
            fprintf(stderr, "Could not retrieve primary message name from id %d\n", (int)id);
            return 0;
        }
        metadata_msgs[imsg].fields_type = (int *)malloc(n_field * sizeof(int));
        if (n_field > 0) {
            if (primary_fields_types_from_id(id, metadata_msgs[imsg].fields_type, n_field) == 0) {
                fprintf(stderr, "Could not retrieve primary fields types from id %d\n", (int)id);
                return 0;
            }
            metadata_msgs[imsg].fields_name = (char **)malloc(n_field * sizeof(char *));
            for (size_t ifield = 0; ifield < n_field; ifield++) {
                metadata_msgs[imsg].fields_name[ifield] = (char *)malloc(GEN_STR_LEN * sizeof(char));
            }
            if (primary_fields_string_from_id(id, (char **)metadata_msgs[imsg].fields_name, n_field, GEN_STR_LEN) ==
                1) {
                fprintf(stderr, "Could not retrieve primary string types from id %d\n", (int)id);
                return 0;
            }
        }
    }
    for (size_t iidx = 0; iidx < CAN_INVERTERS_MESSAGE_COUNT; iidx++) {
        uint16_t id = can_inverters_api_id_from_index(iidx);
        if (id == 0xFF) {
            fatal_error("Could not retrieve inverter message id from index");
        }
        int imsg                     = get_idx_from_intf(iidx, inverter_intf);
        int n_field                  = inverters_n_fields_from_id(id);
        metadata_msgs[imsg].n_fields = n_field;
        metadata_msgs[imsg].msg_name = (char *)malloc(GEN_STR_LEN * sizeof(char));
        if (!inverters_message_name_from_id(id, metadata_msgs[imsg].msg_name)) {
            fprintf(stderr, "Could not retrieve inverters message name from id %d\n", (int)id);
            return 0;
        }
        metadata_msgs[imsg].fields_type = (int *)malloc(n_field * sizeof(int));
        if (n_field > 0) {
            if (inverters_fields_types_from_id(id, metadata_msgs[imsg].fields_type, n_field) == 0) {
                fprintf(stderr, "Could not retrieve inverters fields types from id %d\n", (int)id);
                return 0;
            }
            metadata_msgs[imsg].fields_name = (char **)malloc(n_field * sizeof(char *));
            for (size_t ifield = 0; ifield < n_field; ifield++) {
                metadata_msgs[imsg].fields_name[ifield] = (char *)malloc(GEN_STR_LEN * sizeof(char));
            }
            if (inverters_fields_string_from_id(id, (char **)metadata_msgs[imsg].fields_name, n_field, GEN_STR_LEN) ==
                1) {
                fprintf(stderr, "Could not retrieve inverters string types from id %d\n", (int)id);
                return 0;
            }
        }
    }
    for (size_t iidx = 0; iidx < CAN_BMS_MESSAGE_COUNT; iidx++) {
        uint16_t id = can_bms_api_id_from_index(iidx);
        if (id == 0xFF) {
            fatal_error("Could not retrieve bms message id from index");
        }
        int imsg                     = get_idx_from_intf(iidx, bms_intf);
        int n_field                  = bms_n_fields_from_id(id);
        metadata_msgs[imsg].n_fields = n_field;
        metadata_msgs[imsg].msg_name = (char *)malloc(GEN_STR_LEN * sizeof(char));
        if (!bms_message_name_from_id(id, metadata_msgs[imsg].msg_name)) {
            fprintf(stderr, "Could not retrieve bms message name from id %d\n", (int)id);
            return 0;
        }
        metadata_msgs[imsg].fields_type = (int *)malloc(n_field * sizeof(int));
        if (n_field > 0) {
            if (bms_fields_types_from_id(id, metadata_msgs[imsg].fields_type, n_field) == 0) {
                fprintf(stderr, "Could not retrieve bms fields types from id %d\n", (int)id);
                return 0;
            }
            metadata_msgs[imsg].fields_name = (char **)malloc(n_field * sizeof(char *));
            for (size_t ifield = 0; ifield < n_field; ifield++) {
                metadata_msgs[imsg].fields_name[ifield] = (char *)malloc(GEN_STR_LEN * sizeof(char));
            }
            if (bms_fields_string_from_id(id, (char **)metadata_msgs[imsg].fields_name, n_field, GEN_STR_LEN) == 1) {
                fprintf(stderr, "Could not retrieve bms string types from id %d\n", (int)id);
                return 0;
            }
        }
    }
    return 1;
}

void fill_data(can_message_t *msg, char *final_command, const char *device) {
    snprintf(final_command, BUFSIZ, "cansend %s %03X#", device, msg->id);
    for (size_t ic = 0; ic < msg->size; ic++) {
        int clen = strlen(final_command);
        snprintf(final_command + clen, BUFSIZ - clen, "%02X", msg->data[ic]);
    }
}

void send_message(int current_focus) {
    int msgid                 = -1;
    can_message_t msg         = {0};
    char to_serialize[BUFSIZ] = {0};
    char final_command[BUFSIZ];
    int can_send_retval          = 0;
    int current_to_serialize_idx = 0;
    if (current_n_fields > 0) {
        int field_len = strlen(current_fields[0]);
        memcpy(to_serialize + current_to_serialize_idx, current_fields[0], field_len);
        current_to_serialize_idx += field_len;
    }
    for (size_t ifield = 1; ifield < current_n_fields; ifield++) {
        int field_len = strlen(current_fields[ifield]);
        memcpy(to_serialize + current_to_serialize_idx, ",", 1);
        current_to_serialize_idx++;
        memcpy(to_serialize + current_to_serialize_idx, current_fields[ifield], field_len);
        current_to_serialize_idx += field_len;
    }
    to_serialize[current_to_serialize_idx] = '\0';
    switch (chosen_intf) {
        case primary_intf: {
            msgid         = can_primary_api_id_from_index(chosen_msg_idx);
            msg.id        = msgid;
            size_t msg_sz = 0;
            (void)primary_serialize_from_string(msgid, to_serialize, msg.data, &msg_sz);
            msg.size = (uint8_t)msg_sz;
            fill_data(&msg, (char *)final_command, (char *)can_devices[0]);
            can_send_retval = can_send(&msg, can_sockets[0]);
            break;
        }
        case inverter_intf: {
            msgid         = can_inverters_api_id_from_index(chosen_msg_idx - get_intf_base_idx());
            msg.id        = msgid;
            size_t msg_sz = 0;
            (void)inverters_serialize_from_string(msgid, to_serialize, msg.data, &msg_sz);
            msg.size = (uint8_t)msg_sz;
            fill_data(&msg, (char *)final_command, (char *)can_devices[0]);
            can_send_retval = can_send(&msg, can_sockets[0]);
            break;
        }
        case bms_intf: {
            msgid         = can_bms_api_id_from_index(chosen_msg_idx);
            msg.id        = msgid;
            size_t msg_sz = 0;
            (void)bms_serialize_from_string(msgid, to_serialize, msg.data, &msg_sz);
            msg.size = (uint8_t)msg_sz;
            fill_data(&msg, (char *)final_command, (char *)can_devices[0]);
            can_send_retval = can_send(&msg, can_sockets[0]);
            break;
        }
        default:
            // TODO: Handle invalid networks
            break;
    }
    WINDOW *prompt_win =
        newwin(SUBWIN_HEIGHT, SUBWIN_WIDTH, (scrheight - SUBWIN_HEIGHT) / 2, (scrwidth - SUBWIN_WIDTH) / 2);
    if (prompt_win == NULL) {
        assert(0 && "Could not create a input prompt window");
    }
    wclear(prompt_win);
    box(prompt_win, 0, 0);
    int current_row = 1;
    if (can_send_retval < 0) {
        WRITE_CENTERED(prompt_win, current_row, "[ERROR]: Could not send message", -1);
    } else {
        WRITE_CENTERED(prompt_win, current_row, "Message was sent successfully", -1);
    }
    current_row++;
    WRITE_CENTERED(prompt_win, current_row, "Manual cansend command [COULD NOT WORK]", -1);
    current_row++;
    WRITE_CENTERED(prompt_win, current_row, final_command, -1);
    current_row++;
    wrefresh(prompt_win);
    getch();
    delwin(prompt_win);
}

int action(int current_focus) {
    switch (ctab) {
        case search_menu:
            // chosen_msg_idx = current_msg_idx + get_intf_base_idx(); // chosen_msg_idx already set
            chosen_intf      = chosen_intf;
            current_n_fields = metadata_msgs[chosen_msg_idx].n_fields;
            ctab             = fill_fields_menu;
            for (size_t ifield = 0; ifield < MAX_N_FIELDS; ifield++) {
                memcpy(current_fields[ifield], "0", 2);
            }
            return 0;
        case main_menu:
            chosen_msg_idx   = current_focus + get_intf_base_idx();
            chosen_interface = chosen_intf;
            current_n_fields = metadata_msgs[chosen_msg_idx].n_fields;
            ctab             = fill_fields_menu;
            for (size_t ifield = 0; ifield < MAX_N_FIELDS; ifield++) {
                memcpy(current_fields[ifield], "0", 2);
            }
            return 0;
        case fill_fields_menu:
            if (current_focus < current_n_fields) {
                retrieve_input_prompt(current_focus);
                return current_focus;
            } else {
                send_message(current_focus);
                chosen_msg_idx   = -1;
                chosen_interface = primary_intf;
                current_n_fields = -1;
                ctab             = main_menu;
                current_focus    = 0;
                return current_focus;
            }
        case can_dump: {
            if (get_can_msg_index_from_id(can_selected_msg.can_id) >= 0) {
                ctab = can_msg;
            }
            return current_focus;
        }
        default:
            // TODO: Handle invalid networks
            return current_focus;
    }
}

int current_focus_dec(int current_focus) {
    switch (ctab) {
        case search_menu:
        case main_menu:
            return clamp(current_focus - 1, 0, get_intf_tot_msg() - 1);
        case fill_fields_menu:
            return clamp(current_focus - 1, 0, metadata_msgs[chosen_msg_idx].n_fields);
        case can_dump:
            return clamp(current_focus - 1, 0, msg_log_count - 1);
        case can_msg: {
            int index = get_can_msg_index_from_id(can_selected_msg.can_id);
            if (index < 0) {
                return current_focus;
            }
            return clamp(current_focus - 1, 0, metadata_msgs[index].n_fields - 1);
        }
        default:
            return current_focus;
    }
}

int current_focus_inc(int current_focus) {
    switch (ctab) {
        case search_menu:
        case main_menu:
            return clamp(current_focus + 1, 0, get_intf_tot_msg() - 1);
        case fill_fields_menu:
            return clamp(current_focus + 1, 0, metadata_msgs[chosen_msg_idx].n_fields);
        case can_dump:
            return clamp(current_focus + 1, 0, msg_log_count - 1);
        case can_msg: {
            int index = get_can_msg_index_from_id(can_selected_msg.can_id);
            if (index < 0) {
                return current_focus;
            }
            return clamp(current_focus + 1, 0, metadata_msgs[index].n_fields - 1);
        }
        default:
            return current_focus;
    }
}

int retrieve_input_prompt(int current_focus) {
    char input_prompt[GEN_STR_LEN] = {0};
    int cind = 0, input_enter = 0;
    WINDOW *prompt_win =
        newwin(SUBWIN_HEIGHT, SUBWIN_WIDTH, (scrheight - SUBWIN_HEIGHT) / 2, (scrwidth - SUBWIN_WIDTH) / 2);
    if (prompt_win == NULL) {
        assert(0 && "Could not create a input prompt window");
    }
    curs_set(1);

    while (cind < GEN_STR_LEN && !input_enter) {
        wclear(prompt_win);
        box(prompt_win, 0, 0);
        int current_row = 1;
        WRITE_CENTERED(prompt_win, current_row, "Enter the value for that field", -1);
        current_row++;
        WRITE_CENTERED(prompt_win, current_row, input_prompt, -1);
        wrefresh(prompt_win);
        int inchar = getch();
        switch (inchar) {
            case ESCAPE_KEY: {
                input_enter = true;
                break;
            }
            case KEY_ENTER:
            case '\n':
                input_enter = true;
                break;
            case KEY_BACKSPACE:
            case 127:
            case '\b':
                cind               = clamp(cind - 1, 0, cind);
                input_prompt[cind] = 0;
                break;
            default:
                input_prompt[cind] = inchar;
                cind++;
                break;
        }
    }
    curs_set(0);
    memcpy(current_fields[current_focus], input_prompt, GEN_STR_LEN);
    delwin(prompt_win);
    return 0U;
}

int render_main_menu(int current_focus, void *data) {
    bool is_main_menu = (data == NULL);

    wclear(stdscr);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
    int current_row = 0;
    box(stdscr, 0, 0);

    const char title[] = "CUSTOM MESSAGE SENDER MENU";
    wattrset(stdscr, COLOR_PAIR(TITLE_THEME));
    current_row++;
    WRITE_CENTERED(stdscr, current_row, title, -1);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
    if (is_main_menu) {
        PRINT_INDICATIONS(stdscr);
    } else {
        char *searching_string = (char *)data;
        current_row += 2;
        const char searching_tip_1[] = "You are searching for the following string";
        WRITE_CENTERED(stdscr, current_row, searching_tip_1, -1);
        current_row++;
        WRITE_CENTERED(stdscr, current_row, searching_string, -1);
        current_row += 2;
    }

    int intf_tot_msg      = get_intf_tot_msg();
    int intf_base_idx     = get_intf_base_idx();
    size_t actual_written = 0;

    for (size_t iidx = 0; actual_written < (n_shown_msgs) && (iidx + current_focus) < intf_tot_msg; iidx++) {
        int showed_msg = current_focus + iidx;
        char *msg_name = metadata_msgs[intf_base_idx + showed_msg].msg_name;
        if (is_main_menu) {
            WRITE_CENTERED(stdscr, current_row + iidx, msg_name, showed_msg);
            if (_is_focused) {
                chosen_msg_idx = intf_base_idx + showed_msg;
            }
            actual_written++;
        } else {
            char *searching_string = (char *)data;
            if (!(strstr(msg_name, searching_string) == NULL)) {
                WRITE_CENTERED(stdscr, current_row + actual_written, msg_name, actual_written);
                if (_is_focused) {
                    chosen_msg_idx = intf_base_idx + showed_msg;
                }
                actual_written++;
            }
        }
    }
    refresh();
    return 0U;
}

int render_fill_fields_menu(int current_focus, void *data) {
    wclear(stdscr);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
    box(stdscr, 0, 0);
    int current_row = 0;

    const char title[] = "MESSAGE FILL FIELDS MENU";
    wattrset(stdscr, COLOR_PAIR(TITLE_THEME));
    current_row++;
    WRITE_CENTERED(stdscr, current_row, title, -1);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
    PRINT_INDICATIONS(stdscr);

    char chosen_msg_label[GEN_LABEL_LEN];
    snprintf(chosen_msg_label, GEN_LABEL_LEN, "Chosen msg name: %s", metadata_msgs[chosen_msg_idx].msg_name);
    wattrset(stdscr, COLOR_PAIR(TITLE_THEME));
    WRITE_CENTERED(stdscr, current_row, chosen_msg_label, -1);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
    current_row += 2;

    const char first_table_row[] = "Available fields";
    WRITE_CENTERED(stdscr, current_row, first_table_row, -1);
    current_row++;
    mvwhline(stdscr, current_row, (scrwidth - SEPARATOR_LEN) / 2, '~', SEPARATOR_LEN);
    current_row++;
    int n_fields = metadata_msgs[chosen_msg_idx].n_fields;
    for (size_t ifield = current_focus; ifield < n_fields && ifield < (10 + current_focus); ifield++) {
        char *field_name                = metadata_msgs[chosen_msg_idx].fields_name[ifield];
        int field_type                  = metadata_msgs[chosen_msg_idx].fields_type[ifield];
        char field_label[GEN_LABEL_LEN] = {0};
        snprintf(field_label, GEN_LABEL_LEN, "%s = %s", field_name, current_fields[ifield]);
        WRITE_CENTERED(stdscr, current_row, field_label, ifield);
        current_row++;
    }
    const char label_send_message[] = "Send this message";
    WRITE_CENTERED(stdscr, current_row, label_send_message, n_fields);
    current_row++;
    return 0;
}

int get_can_msg_index_from_id(int id) {
    int idx = -1;
    switch (chosen_intf) {
        case primary_intf:
            idx = can_primary_api_index_from_id(id);
            break;
        case bms_intf:
            idx = can_bms_api_index_from_id(id);
            break;
        case inverter_intf:
            idx = can_inverters_api_index_from_id(id);
            break;
        default:
            break;
    }

    if (idx < 0) {
        return -1;
    }

    return idx + get_intf_base_idx();
}

int render_can_msg_center(WINDOW *win, int row, struct can_frame *msg, int current_focus) {
    int msg_idx = get_can_msg_index_from_id(msg->can_id);

    if (msg_idx >= 0) {
        char str[200];
        sprintf(str, "[0x%03X] %s", msg->can_id, metadata_msgs[msg_idx].msg_name);
        WRITE_CENTERED(win, row, "                                          ", -1);
        WRITE_CENTERED(win, row, str, row - 12);

        if (row - 12 == current_focus)
            can_selected_msg = *msg;
        return 0;
    }

    return -1;
}

int render_can_dump(int current_focus, void *data) {
    wclear(stdscr);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
    box(stdscr, 0, 0);
    int current_row = 0;

    const char title[] = "CAN DUMP MENU";
    wattrset(stdscr, COLOR_PAIR(TITLE_THEME));
    current_row++;
    WRITE_CENTERED(stdscr, current_row, title, -1);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));

    PRINT_INDICATIONS(stdscr);

    render_can_dump_msg(current_focus, data);

    refresh();

    return 0;
}

int render_can_dump_msg(int current_focus, void *data) {
    int current_row = 12;
    int msg_count   = 0;
    int msg_invalid = 0;
    for (size_t i = 0; i < CAN_PRIMARY_MESSAGE_COUNT + CAN_INVERTERS_MESSAGE_COUNT + CAN_BMS_MESSAGE_COUNT; ++i) {
        if (msg_log[i].rec) {
            if (msg_count >= current_focus) {
                render_can_msg_center(stdscr, current_row++, &msg_log[i].data, current_focus - msg_count);
            }
            msg_count++;
        }
    }

    return 0;
}

int render_can_msg_data_fields(int current_focus) {
    int msg_idx           = 0;
    char msg_params[1024] = {0};

    switch (chosen_intf) {
        case primary_intf:
            if (!can_primary_api_id_is_valid(can_selected_msg.can_id))
                return -1;
            msg_idx = can_primary_api_index_from_id(can_selected_msg.can_id);
            (void)can_primary_api_deserialize_from_id(
                can_selected_msg.can_id, can_selected_msg.data, &can_network_message.can_primary_message);

            // TODO: Change
            primary_to_string_from_id(can_selected_msg.can_id, &can_network_message.can_primary_message, msg_params);
            break;
        case bms_intf:
            if (!can_bms_api_id_is_valid(can_selected_msg.can_id))
                return -1;
            msg_idx = can_bms_api_index_from_id(can_selected_msg.can_id);
            (void)can_bms_api_deserialize_from_id(
                can_selected_msg.can_id, can_selected_msg.data, &can_network_message.can_bms_message);

            // TODO: Change
            bms_to_string_from_id(can_selected_msg.can_id, rxdev.message, msg_params);
            break;
        case inverter_intf:
            if (!can_inverters_api_id_is_valid(can_selected_msg.can_id))
                return -1;
            msg_idx = can_inverters_api_index_from_id(can_selected_msg.can_id);
            (void)can_inverters_api_deserialize_from_id(
                can_selected_msg.can_id, can_selected_msg.data, &can_network_message.can_inverters_message);

            // TODO: Change
            inverters_to_string_from_id(can_selected_msg.can_id, rxdev.message, msg_params);
            break;
        default:
            break;
    }

    char *token, *ptr, *tmpstr = msg_params;
    int msg_count   = 0;
    int current_row = 12;
    for (size_t i = 0; i < metadata_msgs[msg_idx].n_fields; i++, tmpstr = NULL) {
        token = strtok_r(tmpstr, ",", (char **restrict)&ptr);
        if (msg_count >= current_focus) {
            char msg_str[1024] = {0};
            sprintf(msg_str, "%s: %s", metadata_msgs[msg_idx].fields_name[i], token);
            WRITE_CENTERED(stdscr, current_row, "                                          ", -1);
            WRITE_CENTERED(stdscr, current_row++, msg_str, msg_count);
        }
        msg_count++;
    }
    return 0;
}

int render_can_msg_data(int current_focus, void *data) {
    wclear(stdscr);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
    box(stdscr, 0, 0);
    int current_row = 0;

    int msg_idx = get_can_msg_index_from_id(can_selected_msg.can_id);
    if (msg_idx < 0) {
        wattrset(stdscr, COLOR_PAIR(TITLE_THEME));
        current_row++;
        WRITE_CENTERED(stdscr, current_row, "No message selected", -1);
        wattrset(stdscr, COLOR_PAIR(MAIN_THEME));
        PRINT_INDICATIONS(stdscr);
        refresh();
        return -1;
    }

    wattrset(stdscr, COLOR_PAIR(TITLE_THEME));
    current_row++;
    WRITE_CENTERED(stdscr, current_row, metadata_msgs[msg_idx].msg_name, -1);
    wattrset(stdscr, COLOR_PAIR(MAIN_THEME));

    PRINT_INDICATIONS(stdscr);

    render_can_msg_data_fields(current_focus);

    refresh();

    return 0;
}

int (*render_menus[n_application_tabs])(int, void *) = {
    render_main_menu,
    render_fill_fields_menu,
    render_main_menu,
    render_can_dump,
    render_can_msg_data,
    render_can_dump};
