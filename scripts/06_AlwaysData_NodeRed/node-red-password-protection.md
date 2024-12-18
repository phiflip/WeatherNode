
# Node-RED Deployment Guide with Password Protection for Alwaysdata

This guide provides step-by-step instructions to set up **Node-RED** on **Alwaysdata**, configure the `settings.js` file, add password protection, and ensure changes are applied properly.

## Table of Contents

1. [SSH Access](#ssh-access)
2. [Node-RED Installation](#node-red-installation)
3. [Creating a Password Hash](#creating-a-password-hash)
4. [Configuration in `settings.js`](#configuration-in-settingsjs)
5. [Restarting Node-RED and Applying Changes](#restarting-node-red-and-applying-changes)
6. [Troubleshooting](#troubleshooting)

---

## 1. SSH Access

To connect to your Alwaysdata server via SSH, use the following command:

```bash
ssh your-username@ssh2.alwaysdata.com
```

Replace `your-username` with your Alwaysdata username.

### Example:

```bash
ssh phiflip@ssh2.alwaysdata.com
```

---

## 2. Node-RED Installation

Install Node-RED using `npm`:

```bash
npm install -g --unsafe-perm node-red
```

---

## 3. Creating a Password Hash

To protect Node-RED with a password, create a password hash using the `node-red-admin` tool.

1. **Install `node-red-admin`** (if not already installed):

   ```bash
   npm install -g node-red-admin
   ```

2. **Create a Password Hash**:

   ```bash
   node-red-admin hash-pw
   ```

3. **Enter your desired password** and copy the generated hash, which will look like this:

   ```plaintext
   $2b$08$K2y/bGpSTZJmOY.nIKs5Qu2nQjv9frWY.vGyxZSOjxzD3P9Elj2Mu
   ```

---

## 4. Configuration in `settings.js`

1. Open `settings.js`:

   ```bash
   nano ~/.node-red/settings.js
   ```

2. Add the following `adminAuth` configuration and paste your generated hash:

   ```javascript
   adminAuth: {
       type: "credentials",
       users: [{
           username: "admin",
           password: "$2b$08$K2y/bGpSTZJmOY.nIKs5Qu2nQjv9frWY.vGyxZSOjxzD3P9Elj2Mu",
           permissions: "*"
       }]
   },
   ```

3. Ensure `uiHost` is set to allow external connections:

   ```javascript
   uiHost: "::",
   ```

4. **Save and exit** with `Ctrl + X`, then `Y`, and `Enter`.

---

## 5. Restarting Node-RED and Applying Changes

1. **Stop any running Node-RED instances**:

   ```bash
   pkill node-red
   ```

2. **Start Node-RED**:

   ```bash
   node-red
   ```

3. **Reload the Site in Alwaysdata**:

   - Go to **Sites** in your Alwaysdata dashboard.
   - Find your Node-RED site entry and click **“Restart”** or refresh the page to apply the changes.

4. **Access Node-RED** via your domain:

   ```plaintext
   http://your-username.alwaysdata.net
   ```

   Example:

   ```plaintext
   http://phiflip.alwaysdata.net
   ```

5. **Log in** with the username **`admin`** and your password.

---

## 6. Troubleshooting

### Common Issues and Solutions

#### Node-RED Still Binding to `127.0.0.1`

- Ensure `uiHost` in `settings.js` is set to `"::"` or `"0.0.0.0"`:

  ```javascript
  uiHost: "::",
  ```

#### NPM Installation Errors

- **Clear NPM Cache**:

  ```bash
  npm cache clean --force
  ```

- **Check Free Disk Space**:

  ```bash
  df -h
  ```

#### Firewall Blocking Access

Check if a firewall is blocking the port:

- **Allow Port 1880**:

  ```bash
  sudo ufw allow 1880
  ```

---

## 🚀 You're All Set!

Your Node-RED instance on Alwaysdata is now protected with a password. If you encounter any issues, refer to the troubleshooting section or contact Alwaysdata support.
