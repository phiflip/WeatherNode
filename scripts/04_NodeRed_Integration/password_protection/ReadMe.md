
# Node-RED Deployment Guide with Password Protection

This guide provides step-by-step instructions to set up **password protection** on **Node-RED**.

---

## Table of Contents

1. [Creating a Password Hash](#creating-a-password-hash)
2. [Configuration in `settings.js`](#configuration-in-settingsjs)
3. [Restarting Node-RED and Applying Changes](#restarting-node-red-and-applying-changes)
4. [Troubleshooting](#troubleshooting)

---

## 1. Creating a Password Hash

To protect Node-RED with a password, follow these steps:

1. **Generate a Password Hash**:
   ```bash
   node-red admin hash-pw
   ```
2. **Enter your desired password** (the input will not be visible) and press **Enter**.

3. **Copy the generated hash**, which will look like this:
   ```plaintext
   $2b$08$K2y/bGpSxZJmOY.nIKs5Qu2nQjv9frWY.cGyRZSEjxzD3P9Elu2Mj
   ```

---

## 2. Configuration in `settings.js`

1. **Open the `settings.js` file**:
   ```bash
   nano ~/.node-red/settings.js
   ```

2. **Add or update the `adminAuth` configuration** with the following code. Replace `"$2b$08$..."` with your generated hash:

   ```javascript
   adminAuth: {
       type: "credentials",
       users: [{
           username: "admin",
           password: "$2b$08$K2y/bGpSxZJmOY.nIKs5Qu2nQjv9frWY.cGyRZSEjxzD3P9Elu2Mj",  // Replace with your hash
           permissions: "*"
       }]
   },
   ```

3. **Paste the hash** using **right-click** or your terminal's paste function.

4. **Save and exit** the file:
   - Press `Ctrl + X`, then `Y`, and finally `Enter`.

---

## 3. Restarting Node-RED and Applying Changes

1. **Stop any running Node-RED instances**:
   ```bash
   pkill node-red
   ```

2. **Start Node-RED**:
   ```bash
   node-red
   ```

3. **Access Node-RED** via your browser using your domain or IP address:
   ```plaintext
   http://your-username.alwaysdata.net
   ```

4. **Log in** with:
   - **Username**: `admin`
   - **Password**: Your previously entered password.

---

## 4. Troubleshooting

### Common Issues

1. **Cannot Log In**:
   - Double-check the `adminAuth` section in `settings.js`.
   - Ensure the hash is correctly pasted.

2. **Changes Not Applied**:
   - Ensure Node-RED was restarted after editing `settings.js`.
   - Check for syntax errors in `settings.js`.

3. **Permission Denied Errors**:
   - Ensure `settings.js` has the correct permissions:
     ```bash
     chmod 600 ~/.node-red/settings.js
     ```

---
