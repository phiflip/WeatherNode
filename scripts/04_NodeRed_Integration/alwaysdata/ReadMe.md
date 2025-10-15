
# Node-RED Deployment Guide for Alwaysdata

This guide provides step-by-step instructions to set up **Node-RED** on **Alwaysdata**, configure the `settings.js` file, install the **Node-RED Dashboard**, and access your instance via SSH.

## Table of Contents
1. [Creating a Node.js Site on Alwaysdata](#creating-a-nodejs-site-on-alwaysdata)
2. [SSH Access](#ssh-access)
3. [Node-RED Installation](#node-red-installation)
4. [Starting Node-RED and Generating `settings.js`](#starting-node-red-and-generating-settingsjs)
5. [Configuration in `settings.js`](#configuration-in-settingsjs)
6. [Installing Node-RED Dashboard](#installing-node-red-dashboard)
7. [Access Node-RED](#access-node-red)
8. [Troubleshooting](#troubleshooting)

---

## 1. Creating a Node.js Site on Alwaysdata

1. **Log in to Alwaysdata**: Go to your Alwaysdata dashboard.

2. **Navigate to "Web > Sites"**:  
   Go to **Web** and then select **Sites** (or if a page already exists, the setting can be made using the Modify cogwheel icon).

3. **Add a New Site**:  
   - Click on **"Add a site"**.  
   - Select **Node.js** as the site type.

4. **Configure the Site**:  
   - **Address**: Enter your desired domain or subdomain (e.g., `yourusername.alwaysdata.net`).  
   - **Command**:  
     ```bash
     node-red
     ```  

5. **Save the Configuration**: Click **"Create"** to create the site.

6. **Enable SSH Access with a Password**:
   - Go to **Remote access > SSH** in your Alwaysdata dashboard.
   - Select your SSH user and set a secure password.
   - Ensure the option "Enable password-based login" is checked.
   - Save your changes.

---

## 2. SSH Access

To connect to your Alwaysdata server via SSH, use the following command in your terminal:

```bash
ssh your-username@ssh-your-username.alwaysdata.net 
```

Replace `your-username` with your Alwaysdata username.

### Example:

```bash
ssh phiflip@ssh-phiflip.alwaysdata.net 
```

---

## 3. Node-RED Installation

Install Node-RED using `npm`:

```bash
npm install -g --unsafe-perm node-red
```

---

## 4. Starting Node-RED and Generating `settings.js`

To configure Node-RED, the `settings.js` file must first be created. This happens the first time Node-RED is started.

1. **Start Node-RED**:  
   Run the following command to start Node-RED for the first time:
   ```bash
   node-red
   ```

2. **Stop Node-RED**:  
   Once the application starts, press `Ctrl + C` to stop Node-RED. The `settings.js` file will now be generated in the `.node-red` directory.

---

## 5. Configuration in `settings.js`

1. **Locate the `settings.js` File**:  
   The file is typically located in the `.node-red` directory:
   ```bash
   ~/.node-red/settings.js
   ```

2. **Edit the File**:  
   Open it with a text editor:
   ```bash
   nano ~/.node-red/settings.js
   ```

3. **Modify the Host Binding**:  
   Update the following line to allow Node-RED to bind to all network interfaces:
   ```javascript
   uiHost: "::",     // Allows binding to IPv6 and IPv4
   ```

4. **Save and Exit**:  
   Press `Ctrl + X`, then `Y`, and `Enter` to save your changes.

---

## 6. Installing Node-RED Dashboard

To install the **Node-RED Dashboard**:

1. **Stop Node-RED** if it is running:
   ```bash
   pkill node-red
   ```

2. **Install the Dashboard Package**:
   ```bash
   cd ~/.node-red
    ```
    ```bash
   npm install node-red-dashboard
   ```

3. **Start Node-RED**:
   ```bash
   node-red
   ```

---

## 7. Access Node-RED

After starting Node-RED, you can access it via your Alwaysdata domain:

```
http://your-username.alwaysdata.net
```

### Example:

```
http://phiflip.alwaysdata.net
```

---

## 8. Troubleshooting

### Common Issues and Solutions

#### 1. **Node-RED Binding to `127.0.0.1` Instead of `0.0.0.0`**

- Ensure `uiHost` in `settings.js` is set to `"::"`:

  ```javascript
  uiHost: "::",
  ```

#### 2. **NPM Installation Errors**
If npm package installations fail, you must clear the cache afterward to free up space on alwaysdata:

- **Clear NPM Cache**:
  ```bash
  npm cache clean --force
  ```

#### 3. **Port Conflicts**

If Node-RED fails to start due to port conflicts, specify a custom port in the `settings.js` file:
```javascript
uiPort: 1880,  // Replace 1880 with an available port
```
