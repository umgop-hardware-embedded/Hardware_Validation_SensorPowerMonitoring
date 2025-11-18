<details>
<summary><strong>Hardware Validation Summary</strong></summary>
<br>

To understand the real-world impact of reducing upload frequency, we validated our algorithmic results on physical hardware. While earlier experiments used publicly available datasets, real IoT deployments impose strict constraints on battery life and radio duty cycle. This section connects compression behavior to actual sensor lifetime by measuring power consumption on a real BLE platform.

We implemented a prototype using two Nordic nRF52840 development boards—one acting as a BLE peripheral (sensor) and the other as a central (receiver). The peripheral sampled temperature data and transmitted readings using two connectivity modes:

- **Continuous BLE** – the sensor maintains an active connection at all times  
- **Disconnect–Reconnect BLE** – the sensor sends a packet, powers down the radio, and reconnects only for the next upload  

To quantify how sampling and upload behavior affect energy usage, we systematically varied:

- **Sampling frequency** \( f_{\text{sample}} \)  
- **Upload frequency** \( f_{\text{upload}} \)

Each configuration was measured using Nordic’s Power Profiler Kit II (PPK2), allowing us to capture both low-power idle phases and short, high-current transmission spikes.

For every \((f_{\text{sample}}, f_{\text{upload}})\) pair, we recorded the average current \( P_{\text{avg}} \) and fit the linear power model:

\[
P_{\text{avg}} = P_{\text{base}} + f_{\text{sample}} C_s + f_{\text{upload}} C_u.
\]

From this, we extracted:

- **\(P_{\text{base}}\)** — baseline system power  
- **\(C_s\)** — incremental sampling cost  
- **\(C_u\)** — incremental upload cost  

These coefficients were then used to estimate the average power—and resulting sensor lifetime—for each smoothing/modeling algorithm evaluated earlier.

**In summary:** reducing upload frequency is the dominant factor in lowering average current draw, and therefore significantly extends battery life in BLE-based IoT sensors.

</details>
