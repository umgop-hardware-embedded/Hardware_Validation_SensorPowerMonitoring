Google Colab Links for Data Processing and Manipulation: 

<details>
<summary><strong>Hardware Validation Summary</strong></summary>
<br>

To validate whether reduced transmissions translate to real power savings and extended sensor life, we implemented a BLE network consisting of a single sensor and a receiver and measured its power consumption under varied upload and sampling frequencies with two different connectivity setups. Power measurements were obtained using Nordic’s Power Profiler Kit II (PPK2), which provides high-resolution current measurements sampled at 100,000 samples per second to capture the short, rapid spikes during BLE transmissions. 

1) Under continuous BLE operation, the sensor maintained an active connection and produced nine (f_sample, f_upload) data points by uploading every 1 s, 10 s, and 60 s. Using least-squares regression, we obtained approximate values of I_base ≈ 8.57 µA, C_s ≈ 0.6 µC, C_u ≈ 4.80 µC for continuous BLE.
2) In the disconnect–reconnect setup, the sensor sent a packet, immediately disconnected, and disabled advertising, creating two visually distinct regimes — a low-power idle phase and a reconnection-and-upload spike — which allowed us to isolate upload energy and baseline power. We calculated the approximate values of I_base ≈ 2.10 µA, C_s ≈ 0.6 µC, C_u ≈ 1412 µC for the disconnect–reconnect setup. 

These results show that the power baseline and transmission frequency dominate total power consumption and produce a clear crossover: maintaining a continuous connection is more efficient for short transmission intervals, while disconnect–reconnect becomes more efficient at longer intervals. 


<img width="319" height="239" alt="Screenshot 2025-11-13 at 11 26 12 PM" src="https://github.com/user-attachments/assets/e4e96cc1-d090-4020-8e8c-1c63b9368a0e" />

<img width="525" height="222" alt="Screenshot 2025-11-13 at 4 53 26 PM" src="https://github.com/user-attachments/assets/e8c6afbd-1b2b-42c8-9c6c-ade53df2aaf5" />


</details>

<details>
<strong>Implementation</strong>

Power Profiler:




Sidewalk: Amazon SideWalk Implementation Code is on the repo. Requires just one nrf52840 board, and Amazon Device (serving as gateway device, I used a Amazon Echo Pop (https://www.amazon.com/Amazon-Echo-Alexa-fits-anywhere/dp/B09ZXLRRHY)). It shows temperature monitoring in the Amazon Sidewalk app.

<img width="720" height="399" alt="image" src="https://github.com/user-attachments/assets/ed4949e5-0f99-430a-8864-97b837b1da5f" />
Reduce overall power consumption in prj.config settings:
  CONFIG_SIDEWALK_LOG_LEVEL_INF=n  
  CONFIG_SERIAL=n
  CONFIG_CONSOLE=n
  CONFIG_UART_CONSOLE=n
  CONFIG_STDOUT_CONSOLE=n
  CONFIG_USE_SEGGER_RTT=n


nrF Connect for Low Power BLE: 

</details>


