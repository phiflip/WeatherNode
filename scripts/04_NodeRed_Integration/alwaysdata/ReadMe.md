# Node-RED Deployment Guide for Alwaysdata

This guide provides step-by-step instructions to set up **Node-RED** on **Alwaysdata**, install the prepared **FlowFuse Dashboard** package, configure the site, and access your instance via SSH.

## Table of Contents
- [Creating a Node.js Site on Alwaysdata](#creating-a-nodejs-site-on-alwaysdata)
- [SSH Access](#ssh-access)
- [Node-RED Installation](#node-red-installation)
- [Access Node-RED](#access-node-red)
- [Troubleshooting](#troubleshooting)
- [Securing Node-RED with a Password](#securing-node-red-with-a-password)
- [Restarting Your Alwaysdata Site](#restarting-your-alwaysdata-site)

---

## Creating a Node.js Site on Alwaysdata

1. **Log in to Alwaysdata**: Go to your Alwaysdata dashboard.

2. **Navigate to "Web > Sites"**:  
   Go to **Web** and then select **Sites** (or if a page already exists, the setting can be made using the Modify cogwheel icon).

3. **Add or edit the site**:  
   - Select **Node.js** as the site type.
   - Use your Alwaysdata address, e.g. `yourusername.alwaysdata.net`.

4. **Configure the command**:

   Replace `USERNAME` with your Alwaysdata username:

   ```bash
   /home/USERNAME/node-red-app/node_modules/.bin/node-red --userDir /home/USERNAME/node-red-data --port $PORT -D uiHost=$IP
   ```

5. **Disable standby**:  
   Under the site settings, open **Advanced** and set:

   ```text
   Idle time: 0
   ```

   `0` means that the site should not stop because of inactivity.

6. **Save the configuration**.

7. **Enable SSH Access with a Password**:
   - Go to **Remote access > SSH** in your Alwaysdata dashboard.
   - Select your SSH user and set a secure password.
   - Ensure the option **Enable password-based login** is checked.
   - Save your changes.

---

## SSH Access

To connect to your Alwaysdata server via SSH, use:

```bash
ssh USERNAME@ssh-USERNAME.alwaysdata.net
```

Replace `USERNAME` with your Alwaysdata username.

### Example

```bash
ssh philiot@ssh-philiot.alwaysdata.net
```

---

## Node-RED Installation

Node-RED and the FlowFuse Dashboard are provided as prepared archives.

Download the following files:

```text
node-red-app.tar.gz
flowfuse-dashboard.tar.gz
```

### 1. Copy the packages to Alwaysdata

If the files are in your Windows Downloads folder:

```cmd
cd %USERPROFILE%\Downloads
```

Then upload both files:

```bash
scp node-red-app.tar.gz flowfuse-dashboard.tar.gz USERNAME@ssh-USERNAME.alwaysdata.net:~/
```

### 2. Log in again via SSH

```bash
ssh USERNAME@ssh-USERNAME.alwaysdata.net
```

### 3. Extract Node-RED and FlowFuse Dashboard

```bash
cd ~
tar -xzf node-red-app.tar.gz
mkdir -p ~/node-red-data
cd ~/node-red-data
tar -xzf ~/flowfuse-dashboard.tar.gz
```

### Important

Do **not** install Node-RED or Dashboard nodes directly on Alwaysdata using the Palette Manager or `npm install`.

The Alwaysdata Free account has only **256 MB RAM**, and the installation can terminate with:

```text
Killed
```

The prepared Moodle packages avoid this problem.

---

## Access Node-RED

Node-RED:

```text
https://USERNAME.alwaysdata.net
```

FlowFuse Dashboard:

```text
https://USERNAME.alwaysdata.net/dashboard
```

### Example

```text
https://philiot.alwaysdata.net
https://philiot.alwaysdata.net/dashboard
```

---

## Troubleshooting

### Node-RED does not start

Check that the site type is **Node.js** and that the command contains your correct Alwaysdata username:

```bash
/home/USERNAME/node-red-app/node_modules/.bin/node-red --userDir /home/USERNAME/node-red-data --port $PORT -D uiHost=$IP
```

### Site goes into standby

Under **Web > Sites > Advanced**, set:

```text
Idle time: 0
```

### `npm install` or Palette Manager returns `Killed`

Do not install packages directly on the Alwaysdata Free server. Use the prepared Moodle packages instead.

---

## Securing Node-RED with a Password

Protecting your Node-RED editor is strongly recommended.

### 1. Generate a Password Hash

```bash
~/node-red-app/node_modules/.bin/node-red admin hash-pw
```

Enter your desired password. It will not be displayed while typing. Copy the generated hash string.

### 2. Edit the `settings.js` File

```bash
nano ~/node-red-data/settings.js
```

Find the `adminAuth` section. Insert the generated hash and uncomment the relevant lines by removing `//`.

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

Save and exit:

```text
Ctrl + X
Y
Enter
```

### 3. Restart Node-RED

Restart the Alwaysdata site under **Web > Sites**.

Then open:

```text
https://USERNAME.alwaysdata.net
```

You will now be prompted to log in with **admin** and your chosen password.

---

## Restarting Your Alwaysdata Site

After changes in `settings.js`:

1. Go to your **Alwaysdata dashboard**.
2. Navigate to **Web > Sites**.
3. Locate your Node.js site running Node-RED.
4. Click the **Restart** icon on the right-hand side.

This ensures that Node-RED restarts and loads the latest configuration.
