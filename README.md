To understand the real-world impact of reducing upload frequency, we validated our algorithmic results on physical hardware. While the earlier experiments rely on publicly available datasets, IoT deployments impose strict constraints on battery life, communication overhead, and radio duty cycle. The goal of this section is to translate compression behavior into actual sensor lifetime by measuring the power required to sample and transmit temperature data on a real BLE system.

We implemented a prototype using two Nordic nRF52840 development boards—one acting as a BLE peripheral (sensor) and the other as a central (receiver). The peripheral sampled its onboard temperature sensor and transmitted readings using two connectivity modes:

Continuous BLE, where the sensor maintains an active connection at all times

Disconnect–Reconnect BLE, where the sensor sends a packet, turns the radio fully off, and reconnects only for the next transmission

To characterize how sampling and upload rates contribute to energy usage, we systematically varied both:
the sampling frequency 
𝑓
sample
f
sample
	​

, and the upload frequency 
𝑓
upload
f
upload
	​

. Each configuration was measured using Nordic’s Power Profiler Kit II (PPK2), which provided high-resolution current traces for both low-power idle periods and short, high-current transmission spikes.

For each 
(
𝑓
sample
,
𝑓
upload
)
(f
sample
	​

,f
upload
	​

) pair, we collected the average current 
𝑃
avg
P
avg
	​

 and fit the linear model:

𝑃
avg
=
𝑃
base
+
𝑓
sample
𝐶
𝑠
+
𝑓
upload
𝐶
𝑢
.
P
avg
	​

=P
base
	​

+f
sample
	​

C
s
	​

+f
upload
	​

C
u
	​

.

This allowed us to extract the baseline system consumption, the incremental sampling cost, and the cost of each BLE upload for both connectivity strategies. Using these coefficients, we then estimated the average power (and resulting battery lifetime) for every smoothing/modeling algorithm evaluated earlier in the paper.

In short, this hardware validation provides the practical link between algorithmic compression performance and real IoT deployment constraints: lower transmission frequency directly reduces average current draw, extending sensor lifetime—often by years.
