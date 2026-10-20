# Node-RED + FlowFuse Dashboard on alwaysdata

Run **Node-RED** and **FlowFuse Dashboard 2** on an **alwaysdata Free** account. The prebuilt archives avoid installing npm packages on the server, where the 256 MB RAM limit may terminate the installation.

## 1. Create an alwaysdata site

1. Create a free account at [alwaysdata.com](https://www.alwaysdata.com/).
2. In **Web → Sites**, create a site with:

   | Setting | Value |
   | --- | --- |
   | Type | `Node.js` |
   | Command (temporary) | `node-red` |
   | Working directory | `/home/USERNAME/` |
   | Advanced → Idle time | `0` |

   Replace `USERNAME` with your alwaysdata username. The temporary command is only a placeholder. **The site may not work yet.** You will replace it in step 4.
3. In **Remote access → SSH**, enable password login and set your SSH password.

## 2. Download and upload the archives

Download both files from the [tested alwaysdata release (October 2026)](https://github.com/phiflip/WeatherNode/releases/tag/alwaysdata-2026-10):

- [node-red-app.tar.gz](https://github.com/phiflip/WeatherNode/releases/download/alwaysdata-2026-10/node-red-app.tar.gz)
- [flowfuse-dashboard.tar.gz](https://github.com/phiflip/WeatherNode/releases/download/alwaysdata-2026-10/flowfuse-dashboard.tar.gz)

Open **Windows Command Prompt** and go to the folder where you downloaded the archives. **Replace the example path with your actual folder**:

```cmd
cd /d "C:\path\to\Downloads"
```

Then upload both files (replace `USERNAME` with your alwaysdata username):

```cmd
scp node-red-app.tar.gz flowfuse-dashboard.tar.gz USERNAME@ssh-USERNAME.alwaysdata.net:~/
```

**Then connect to your server via SSH:**

```bash
ssh USERNAME@ssh-USERNAME.alwaysdata.net
```

## 3. Extract Node-RED and FlowFuse Dashboard

Run on the **alwaysdata server**:

```bash
cd ~
tar -xzf node-red-app.tar.gz
mkdir -p ~/node-red-data
cd ~/node-red-data
tar -xzf ~/flowfuse-dashboard.tar.gz
```

Do **not** use the Palette Manager or `npm install` to install these packages on alwaysdata Free. The installation may fail due to insufficient RAM (`Killed`).

## 4. Replace the temporary site command on alwaysdata

Go to **Web → Sites → Edit** and replace the temporary `node-red` command with the following **single line** (replace `USERNAME` twice):

```bash
/home/USERNAME/node-red-app/node_modules/.bin/node-red --userDir /home/USERNAME/node-red-data --port $PORT -D uiHost=$IP
```

**Important:** Keep the space before `--port`. Otherwise, Node-RED may use the wrong data directory (for example, `node-red-data--port`) and FlowFuse nodes will be missing.

Save and **restart** the site.

## 5. Open Node-RED

- **Editor:** `https://USERNAME.alwaysdata.net`
- **FlowFuse Dashboard:** `https://USERNAME.alwaysdata.net/dashboard`

Add a dashboard widget (for example, `ui-button`) and click **Deploy**. FlowFuse Dashboard uses `/dashboard`, not the old `/ui` path.

## 6. Protect the Node-RED editor with a password

**Do this only after Node-RED starts successfully.** These steps use two different environments: first the **SSH terminal**, then the **`nano` text editor**.

### 6.1 Generate a password hash (SSH terminal)

Connect to your alwaysdata server via SSH (see step 2). Enter **only this command** in the terminal:

```bash
~/node-red-app/node_modules/.bin/node-red admin hash-pw
```

1. Type a password and press **Enter**. The password is **not displayed** while you type.
2. The terminal prints a long password hash (typically starting with `$2b$`).
3. **Copy the entire hash**. You will paste it into `settings.js` in step 6.2. Do not paste the password itself.

### 6.2 Edit `settings.js` (`nano` editor)

In the **SSH terminal**, open the configuration file:

```bash
nano ~/node-red-data/settings.js
```

**You are now inside the `nano` text editor, not at the terminal prompt.**

1. Press **Ctrl + W**, type `adminAuth`, and press **Enter** to find the `adminAuth` section.
2. Uncomment the `adminAuth` block by removing the `//` at the beginning of each relevant line.
3. Replace the existing example value of `password` with the **hash you copied**. Keep the quotation marks around the hash.
4. The enabled section should look like the example below. It is **JavaScript configuration**, **not a terminal command**. Edit the existing block rather than adding a duplicate.

```javascript
adminAuth: {
    type: "credentials",
    users: [{
        username: "admin",
        password: "PASTE_YOUR_HASH_HERE",
        permissions: "*"
    }]
},
```
In `nano`, press **Ctrl + X**, then **Y**, then **Enter** to save.

### 6.3 Save and restart

1. In the alwaysdata control panel, open **Web → Sites** and **restart** your Node.js site.
2. Open `https://USERNAME.alwaysdata.net`. Sign in with username **`admin`** and the **password you chose in step 6.1**, *not* the hash.

**Security note:** `adminAuth` protects the Node-RED editor and Admin API. It does not automatically password-protect the FlowFuse Dashboard itself.

## Troubleshooting

- **FlowFuse nodes missing:** Check `--userDir`, the space before `--port`, and the file `~/node-red-data/node_modules/@flowfuse/node-red-dashboard/package.json`.
- **npm reports `Killed`:** Use the release archives instead of installing on the server.
- **Site stops when idle:** Set **Advanced → Idle time** to `0`.


**References:** [Node-RED](https://nodered.org/docs/), [FlowFuse Dashboard](https://dashboard.flowfuse.com/), [alwaysdata](https://help.alwaysdata.com/).
