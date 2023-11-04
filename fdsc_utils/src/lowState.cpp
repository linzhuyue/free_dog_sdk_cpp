#include  <fdsc_utils/lowState.hpp>

namespace FDSC
{
    void lowState::dataToBmsState(const std::vector<uint8_t> &data)
    {
        version_h = data[0];
        version_l = data[1];
        bms_status = data[2];
        SOC = data[3];
        current = hex_to_int_i(data, 4, 8);
        cycle = hex_to_u16_i(data, 8, 10);
        BQ_NTC[0] = data[10];
        BQ_NTC[1] = data[11];
        MCU_NTC[0] = data[12];
        MCU_NTC[1] = data[13];

        for (int i = 0; i < 10; i++)
        {
            cell_vol[i] = hex_to_u16_i(data, 13 + i * 2, 15 + i * 2);
        }
    }

    void lowState::dataToWirelessRemoteState(const std::vector<uint8_t> &data)
    {
        wirelessdata.head[0] = data[0];
        wirelessdata.head[1] = data[1];
        wirelessdata.btn.components.R1 = data[2];
        wirelessdata.btn.components.L1 = data[3];
        wirelessdata.btn.components.start = data[4];
        wirelessdata.btn.components.select = data[5];
        wirelessdata.btn.components.R2 = data[6];
        wirelessdata.btn.components.L2 = data[7];
        wirelessdata.btn.components.F1 = data[8];
        wirelessdata.btn.components.F2 = data[9];
        wirelessdata.btn.components.A = data[10];
        wirelessdata.btn.components.B = data[11];
        wirelessdata.btn.components.X = data[12];
        wirelessdata.btn.components.Y = data[13];
        wirelessdata.btn.components.up = data[14];
        wirelessdata.btn.components.right = data[15];
        wirelessdata.btn.components.down = data[16];
        wirelessdata.btn.components.left = data[17];

        wirelessdata.lx = hex_to_float_2B(data, 18, 19);
        wirelessdata.rx = hex_to_float_2B(data, 20, 21);
        wirelessdata.ry = hex_to_float_2B(data, 22, 23);
        wirelessdata.ly = hex_to_float_2B(data, 24, 25); // TODO may be not right. Only use 24 B
    }

    void lowState::dataToImu(const std::vector<uint8_t> &data)
    {
        for (int i = 0; i < 4; i++)
        {
            imu_quaternion[i] = hex_to_float_i(data, i * 4, (i + 1) * 4);
        }
        for (int i = 0; i < 3; i++)
        {
            imu_gyroscope[i] = hex_to_float_i(data, 16 + i * 4, 16 + (i + 1) * 4);
            imu_accelerometer[i] = hex_to_float_i(data, 28 + i * 4, 28 + (i + 1) * 4);
            imu_rpy[i] = hex_to_float_i(data, 40 + i * 4, 40 + (i + 1) * 4);
        }
        temperature_imu = data[52];
    }

    void lowState::dataToMotorState(const std::vector<uint8_t> &data, int index)
    {
        motormode = int(data[0]);
        q = hex_to_float_i(data, 1, 5);
        dq = hex_to_float_i(data, 5, 9);
        ddq = static_cast<float>(hex_to_u16_i(data, 9, 11));
        tauEst = static_cast<float>(hex_to_u16_i(data, 11, 13)) * 0.00390625; // TODO why? Do not understand?
        q_raw = hex_to_float_i(data, 13, 17);
        dq_raw = hex_to_float_i(data, 17, 21);
        ddq_raw = static_cast<float>(hex_to_u16_i(data, 21, 23));
        motor_temperature = data[24];
        std::copy(data.begin() + 24, data.end(), res_bms.begin());
        motorState[index].set_data(motormode, q, dq, ddq, tauEst, q_raw, dq_raw, ddq_raw, motor_temperature, res_bms);
    }

    void lowState::parseData(const std::vector<uint8_t> &data)
    {
        try
        {
            // Data length is 820
            std::copy(data.begin(), data.begin() + 2, head.begin()); // 0-2
            levelFlag = data[2];
            frameReserve = data[3];
            std::copy(data.begin() + 4, data.begin() + 12, SN.begin());      // 4-12
            std::copy(data.begin() + 12, data.begin() + 20, version.begin()); // 12-20
            bandWidth[0] = data[20];
            bandWidth[1] = data[21];
            std::vector<uint8_t> imu_data(data.begin() + 22, data.begin() + 75); // 22-75
            dataToImu(imu_data);

            for (int i = 0; i < 20; i++)
            {
                std::vector<uint8_t> motor_tmp_data(data.begin() + 75 + i * 32, data.begin() + 32 + 75 + i * 32);
                dataToMotorState(motor_tmp_data, i);
            }

            std::vector<uint8_t> bms_data(data.begin() + 715, data.begin() + 739);
            dataToBmsState(bms_data);
            // We do not have this in GO 1 Air
            for (int i = 0; i < 4; i++)
            {
                footForce[i] = hex_to_u16_i(data, 739 + i * 2, 741 + i * 2);
                footForceEst[i] = hex_to_u16_i(data, 747 + i * 2, 749 + i * 2);
            }
            std::copy(data.begin() + 755, data.begin() + 759, robotlowmode.begin()); //
            std::copy(data.begin() + 759, data.begin() + 799, wirelessRemote.begin()); //

            dataToWirelessRemoteState(wirelessRemote);
            std::copy(data.begin() + 799, data.begin() + 803, reserve.begin()); //
            std::copy(data.begin() + 803, data.begin() + 808, crc.begin()); //
            std::copy(data.begin() + 808, data.end(), zeros_res.begin()); // 808-820
        }
        catch (const std::exception &e)
        {
            int count_print = 0;
            for (const auto &byte : data)
            {
                std::cout << " 0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
                count_print++;
                if (count_print % 16 == 0)
                {
                    std::cout << std::endl;
                }
            }
            std::cout << std::dec << std::endl;
            std::cerr << e.what() << '\n';
        }
    }
} // namespace FDSC
