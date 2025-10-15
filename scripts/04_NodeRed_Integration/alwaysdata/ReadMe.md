# Node-RED Deployment Guide for Alwaysdata

This guide provides step-by-step instructions to set up **Node-RED** on **Alwaysdata**, configure the `settings.js` file, install the **Node-RED Dashboard**, and access your instance via SSH.

## Table of Contents
- [Creating a Node.js Site on Alwaysdata](#creating-a-nodejs-site-on-alwaysdata)
- [SSH Access](#ssh-access)
- [Node-RED Installation](#node-red-installation)
- [Starting Node-RED and Generating settings.js](#starting-node-red-and-generating-settingsjs)
- [Configuration in settings.js](#configuration-in-settingsjs)
- [Installing Node-RED Dashboard](#installing-node-red-dashboard)
- [Access Node-RED](#access-node-red)
- [Troubleshooting](#troubleshooting)
- [Securing Node-RED with a Password](#securing-node-red-with-a-password)

---

## Creating a Node.js Site on Alwaysdata

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

## SSH Access

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

## Node-RED Installation

Install Node-RED using `npm`:

```bash
npm install -g --unsafe-perm node-red
```

---

## Starting Node-RED and Generating settings.js

To configure Node-RED, the `settings.js` file must first be created. This happens the first time Node-RED is started.

1. **Start Node-RED**:  
   Run the following command to start Node-RED for the first time:
   ```bash
   node-red
   ```

2. **Stop Node-RED**:  
   Once the application starts, press `Ctrl + C` to stop Node-RED. The `settings.js` file will now be generated in the `.node-red` directory.

---

## Configuration in settings.js

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

## Installing Node-RED Dashboard

To install the **Node-RED Dashboard**:

1. **Stop Node-RED** if it is running:
   ```bash
   pkill node-red
   ```

2. **Install the Dashboard Package**:
   ```bash
   cd ~/.node-red
   npm install node-red-dashboard
   ```

3. **Start Node-RED**:
   ```bash
   node-red
   ```

---

## Access Node-RED

After starting Node-RED, you can access it via your Alwaysdata domain:

```
http://your-username.alwaysdata.net
```

### Example:

```
http://phiflip.alwaysdata.net
```

---

## Troubleshooting

### Common Issues and Solutions

#### Node-RED Binding to `127.0.0.1` Instead of `0.0.0.0`

- Ensure `uiHost` in `settings.js` is set to `"::"`:

  ```javascript
  uiHost: "::",
  ```

#### NPM Installation Errors
If npm package installations fail, you must clear the cache afterward to free up space on alwaysdata:

- **Clear NPM Cache**:
  ```bash
  npm cache clean --force
  ```

#### Port Conflicts

If Node-RED fails to start due to port conflicts, specify a custom port in the `settings.js` file:
```javascript
uiPort: 1880,  // Replace 1880 with an available port
```

---

## Securing Node-RED with a Password

Protecting your Node-RED editor is strongly recommended.

### 1. Generate a Password Hash

Run the following command and enter your desired password (it will not be displayed while typing):

```bash
node-red admin hash-pw
```

Copy the generated hash string (it starts with `$2b$...`).

---

### 2. Edit the `settings.js` File

Open your configuration file:

```bash
nano ~/.node-red/settings.js
```

Find the `adminAuth` section (it may be commented out) and replace it with the following:

```javascript
adminAuth: {
    type: "credentials",
    users: [{
        username: "admin",
        password: "<PASTE-YOUR-HASH-HERE>",
        permissions: "*"
    }]
},
```

Save and exit (`Ctrl + X`, then `Y`, and `Enter`).

---

### 3. Restart Node-RED

```bash
node-red
```

After restarting, open your Node-RED instance again in the browser:

```
https://your-username.alwaysdata.net
```

You will now be prompted to log in with **admin** and your chosen password.
