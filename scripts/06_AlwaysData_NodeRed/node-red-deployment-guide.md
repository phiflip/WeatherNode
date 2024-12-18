
# Node-RED Deployment Guide for Alwaysdata

This guide provides step-by-step instructions to set up **Node-RED** on **Alwaysdata**, configure the `settings.js` file, install the **Node-RED Dashboard**, and access your instance via SSH.

## Table of Contents

1. [SSH Access](#ssh-access)
2. [Node-RED Installation](#node-red-installation)
3. [Configuration in `settings.js`](#configuration-in-settingsjs)
4. [Service and Site Commands](#service-and-site-commands)
5. [Installing Node-RED Dashboard](#installing-node-red-dashboard)
6. [Troubleshooting](#troubleshooting)

---

## 1. SSH Access

To connect to your Alwaysdata server via SSH, use the following command:

```bash
ssh your-username@ssh-your-username.alwaysdata.net 
```

Replace `your-username` with your Alwaysdata username.

### Example:

```bash
ssh phiflip@phiflip.alwaysdata.net 
```
---

## 2. Node-RED Installation

Install Node-RED using `npm`:

```bash
npm install -g --unsafe-perm node-red
```
---
## 3. Configuration in `settings.js`

Configure the `settings.js` file to allow Node-RED to bind to all network interfaces.

1. Open `settings.js`:

   ```bash
   nano ~/.node-red/settings.js
   ```

2. Modify the following lines:

   ```javascript
   uiHost: "::",     // Allows binding to IPv6 and IPv4
   ```
3. **Save and exit** with `Ctrl + X`, then `Y`, and `Enter`.
---

## 4. Site Commands in Alwaysdata

### Site Command

In Alwaysdata's **Sites Configuration**, set the command to:

```bash
node-red
```

---

## 5. Installing Node-RED Dashboard

To install the **Node-RED Dashboard**:

1. **Stop Node-RED** if it is running:

   ```bash
   pkill node-red
   ```

2. Install the dashboard package:

   ```bash
   cd ~/.node-red
   npm install node-red-dashboard@3.6.5
   ```

3. **Start Node-RED**:

   ```bash
   node-red
   ```

---

## 6. Troubleshooting

### Common Issues and Solutions

#### 1. **Node-RED Binding to `127.0.0.1` Instead of `0.0.0.0`**

- Ensure `uiHost` in `settings.js` is set to `"::"`:

  ```javascript
  uiHost: "::",
  ```

#### 2. **NPM Installation Errors**

If you encounter errors during installation, try:

- **Clear NPM Cache**:

  ```bash
  npm cache clean --force
  ```

## 7. Access Node-RED

After starting Node-RED, access it via your domain:

```
http://your-username.alwaysdata.net
```

Example:

```
http://phiflip.alwaysdata.net
```

---
## 🚀 You're All Set!

