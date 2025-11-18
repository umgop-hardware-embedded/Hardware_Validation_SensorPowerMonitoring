Hardware Validation Summary
To understand the real-world impact of reducing upload frequency, we validated our algorithmic results on physical hardware. While earlier experiments used publicly available datasets, real IoT deployments impose strict constraints on battery life and radio duty cycle. This section connects compression behavior to actual sensor lifetime by measuring power consumption on a real BLE platform.
We implemented a prototype using two Nordic nRF52840 development boards—one acting as a BLE peripheral (sensor) and the other as a central (receiver). The peripheral sampled temperature data and transmitted readings using two connectivity modes:
Continuous BLE: the sensor maintains an active connection at all times


Disconnect–Reconnect BLE: the sensor sends a packet, powers down the radio, and reconnects only for the next upload


To quantify how sampling and upload behavior affect energy usage, we systematically varied:
Sampling frequency fsamplef_{\text{sample}}fsample​


Upload frequency fuploadf_{\text{upload}}fupload​


Each configuration was measured using Nordic’s Power Profiler Kit II (PPK2), allowing us to capture both low-power idle phases and short, high-current transmission spikes.
For every (fsample,fupload)(f_{\text{sample}}, f_{\text{upload}})(fsample​,fupload​) pair, we recorded the average current PavgP_{\text{avg}}Pavg​ and fit the linear model:
Pavg=Pbase+fsampleCs+fuploadCu.P_{\text{avg}} = P_{\text{base}} + f_{\text{sample}} C_s + f_{\text{upload}} C_u.Pavg​=Pbase​+fsample​Cs​+fupload​Cu​.
From this, we extracted:
PbaseP_{\text{base}}Pbase​ — baseline system power


CsC_sCs​ — incremental sampling cost


CuC_uCu​ — incremental upload cost


These coefficients were then used to estimate the average power—and resulting battery lifetime—for each smoothing/modeling algorithm evaluated earlier.
In summary: reducing upload frequency is the dominant factor in lowering average current draw, and therefore significantly extends battery life in BLE-based IoT sensors.

