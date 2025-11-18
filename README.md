<details>
<summary><strong>Hardware Validation Summary</strong></summary>
<br>

To validate whether reduced transmissions translate to real power savings and extended sensor life, we implemented a BLE network consisting of a single sensor and a receiver and measured its power consumption under varied upload and sampling frequencies with two different connectivity setups. Power measurements were obtained using Nordic’s Power Profiler Kit II (PPK2), which provides high-resolution current measurements sampled at 100,000 samples per second to capture the short, rapid spikes during BLE transmissions. 

1) Under continuous BLE operation, the sensor maintained an active connection and produced nine (f_sample, f_upload) data points by uploading every 1 s, 10 s, and 60 s. Using least-squares regression, we obtained approximate values of I_base ≈ 8.57 µA, C_s ≈ 0.6 µC, C_u ≈ 4.80 µC for continuous BLE.
2) In the disconnect–reconnect setup, the sensor sent a packet, immediately disconnected, and disabled advertising, creating two visually distinct regimes — a low-power idle phase and a reconnection-and-upload spike — which allowed us to isolate upload energy and baseline power. We calculated the approximate values of I_base ≈ 2.10 µA, C_s ≈ 0.6 µC, C_u ≈ 1412 µC for the disconnect–reconnect setup. 

These results show that the power baseline and transmission frequency dominate total power consumption and produce a clear crossover: maintaining a continuous connection is more efficient for short transmission intervals, while disconnect–reconnect becomes more efficient at longer intervals. More info can be found on our paper: Data Trickles: A Study on Reducing Transmissions
for Live, Low-Data-Rate Data Streams.

</details>
