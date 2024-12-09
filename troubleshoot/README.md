# Troubleshooting Guide for WeatherNode

Welcome to the Troubleshooting section of the WeatherNode repository. This guide is intended to help you diagnose and resolve common issues that may arise when working with the WeatherNode project. Below you will find descriptions of known issues, their causes, and recommended solutions. For each issue, there may also be example scripts provided to assist with troubleshooting.

## Table of Contents


1. [Issue: CubeCell Not Recognized or No Port Assigned in Arduino IDE](#issue-cubecell-not-recognized-or-no-port-assigned-in-arduino-ide)
2. [Issue: Node-RED Requires Superuser Privileges on macOS](#issue-node-red-requires-superuser-privileges-on-macos)
3. [Issue: PowerShell Script Error on Windows During Node.js/Node-RED Installation](#issue-powershell-script-error-on-windows-during-nodejsnode-red-installation)
4. [Issue: Problems with Git When Installing Additional Packages](#issue-problems-with-git-when-installing-additional-packages)

---

## Issue: CubeCell Not Recognized or No Port Assigned in Arduino IDE

**Description:**  
In some cases, especially during the initial setup (e.g., Week 1), the CubeCell board may not be recognized by the Arduino IDE, or no port is assigned.

**Cause:**  
This issue is typically caused by a missing or incorrect driver for the CP2102 USB-to-UART bridge used by the CubeCell.

**Solution:**  
Windows users need to install the CP2102 driver. Follow these steps:
1. Download and install the CP2102 driver from the official source: [CP2102 Driver](https://www.pololu.com/docs/0J7/all#2).
2. After installation, restart your computer.
3. Reconnect the CubeCell board and open the Arduino IDE.
4. Check if the port is now correctly assigned under `Tools > Port`.

**Additional Tips:**
- If the port still doesn't appear, try using a different USB cable or USB port on your computer.
- Ensure that the CubeCell board is properly powered and connected.

---
## Issue: Node-RED Requires Superuser Privileges on macOS

**Description:**  
On macOS, when starting Node-RED, you may encounter issues with installing packages from the palette if you do not have sufficient privileges.

**Cause:**  
This occurs because Node-RED needs superuser privileges to modify system-level files and install packages.

**Solution:**  
To resolve this issue, start Node-RED with superuser privileges by using the following command in the terminal:
```bash
sudo node-red
```
---
## Issue: PowerShell Script Error on Windows During Node.js/Node-RED Installation

**Description:**  
On Windows, you might encounter a red error message during the installation of Node.js or when running Node-RED, related to a PowerShell script.

**Cause:**  
This problem is caused by a restrictive policy that prevents certain scripts from running, or an issue with the `.ps1` PowerShell script.

**Solution:**  
Navigate to the installation directory where the PowerShell script is located (typically `C:/Users/[YourUserName]/AppData/Roaming/npm`), and rename the `.ps1` script file to something else, for example:

```bash
Rename "your_script.ps1" to "your_script.pskill"
```
---
## Issue: Problems with Git When Installing Additional Packages

**Description:**  
When trying to install additional packages for your project, you may encounter issues if Git is not installed on your system.

**Cause:**  
Git is required for downloading and installing certain Node.js/Node-RED packages that rely on Git repositories.

**Solution:**  
Ensure that Git is installed on your system. You can download and install Git from the official website: [Git Downloads](https://git-scm.com/downloads).  
After installation, verify that Git is correctly installed by running the following command in your terminal or command prompt:

```bash
git --version
```
**Note:** This guide is a living document and will be updated as new issues and solutions are identified. Always check for the latest version in the repository.

