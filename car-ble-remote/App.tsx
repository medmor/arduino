import React, {useState, useRef, useCallback} from 'react';
import {
  SafeAreaView,
  StatusBar,
  StyleSheet,
  Text,
  View,
  TouchableOpacity,
  PermissionsAndroid,
  Platform,
} from 'react-native';
import { BleManager, Device } from 'react-native-ble-plx';
type AndroidPermission = (typeof PermissionsAndroid.PERMISSIONS)[keyof typeof PermissionsAndroid.PERMISSIONS];

// Keep in sync with esp32c3_car_ble/esp32c3_car_ble.ino
const SERVICE_UUID = '4fafc201-1fb5-459e-8fcc-c5c9c331914b';
const CHARACTERISTIC_UUID = 'beb5483e-36e1-4688-b7f5-ea07361f1196';
const TARGET_NAME = 'ESP32C3-Car-BLE';

type ConnState = 'idle' | 'scanning' | 'connecting' | 'connected';

type DirButtonProps = {
  label: string;
  cmd: string;
  disabled: boolean;
  onPress: (cmd: string) => void;
  onRelease: (cmd: string) => void;
  style?: object;
};

function DirButton({label, cmd, disabled, onPress, onRelease, style}: DirButtonProps) {
  const [pressed, setPressed] = useState(false);
  const handleStart = () => {
    if (disabled) {
      return;
    }
    setPressed(true);
    onPress(cmd);
  };
  const handleEnd = () => {
    setPressed(false);
    onRelease(cmd);
  };
  return (
    <View
      style={[styles.btn, style, pressed && styles.btnPressed]}
      onTouchStart={handleStart}
      onTouchEnd={handleEnd}
      onTouchCancel={handleEnd}>
      <Text style={styles.btnText}>{label}</Text>
    </View>
  );
}

export default function App() {
  const managerRef = useRef<BleManager | null>(null);
  const deviceRef = useRef<Device | null>(null);
  const [conn, setConn] = useState<ConnState>('idle');
  const [status, setStatus] = useState<string>('Not connected');

  const ensureManager = () => {
    if (!managerRef.current) {
      managerRef.current = new BleManager();
    }
    return managerRef.current;
  };

  const requestPermissions = async (): Promise<boolean> => {
    if (Platform.OS !== 'android') {
      return true;
    }
    const apiLevel = Platform.Version as number;
    const perms: AndroidPermission[] =
      apiLevel >= 31
        ? [
            PermissionsAndroid.PERMISSIONS.BLUETOOTH_SCAN,
            PermissionsAndroid.PERMISSIONS.BLUETOOTH_CONNECT,
            PermissionsAndroid.PERMISSIONS.ACCESS_FINE_LOCATION,
          ]
        : [PermissionsAndroid.PERMISSIONS.ACCESS_FINE_LOCATION];

    const granted = await PermissionsAndroid.requestMultiple(perms);
    return Object.values(granted).every(
      v => v === PermissionsAndroid.RESULTS.GRANTED,
    );
  };

  const connect = useCallback(async () => {
    try {
      const ok = await requestPermissions();
      if (!ok) {
        setStatus('Bluetooth permissions denied');
        return;
      }

      const mgr = ensureManager();
      setConn('scanning');
      setStatus('Scanning...');

      mgr.stopDeviceScan();

      mgr.startDeviceScan(
        [SERVICE_UUID],
        {allowDuplicates: false},
        async (err, device) => {
          if (err || !device) {
            return;
          }
          const name = device.name ?? device.localName ?? '';
          if (!name.includes(TARGET_NAME)) {
            return;
          }
          mgr.stopDeviceScan();

          try {
            setConn('connecting');
            setStatus(`Connecting to ${name}...`);
            await device.connect({autoConnect: false});
            await device.discoverAllServicesAndCharacteristics();
            deviceRef.current = device;
            setConn('connected');
            setStatus(`Connected to ${name}`);
          } catch (e: any) {
            setConn('idle');
            setStatus(`Connect failed: ${e?.message ?? String(e)}`);
          }
        },
      );

      setTimeout(() => {
        if (deviceRef.current == null) {
          mgr.stopDeviceScan();
          setConn('idle');
          setStatus(TARGET_NAME + ' not found');
        }
      }, 10000);
    } catch (e: any) {
      setConn('idle');
      setStatus(`Scan failed: ${e?.message ?? String(e)}`);
    }
  }, []);

  const send = useCallback(async (cmd: string) => {
    const dev = deviceRef.current;
    if (!dev) {
      setStatus('Not connected');
      return;
    }
    try {
      const valueBase64 = Buffer.from(cmd, 'utf8').toString('base64');
      await dev.writeCharacteristicWithResponseForService(
        SERVICE_UUID,
        CHARACTERISTIC_UUID,
        valueBase64,
      );
      setStatus(`Sent: ${cmd}`);
    } catch (e: any) {
      setStatus(`Send failed: ${e?.message ?? String(e)}`);
      setConn('idle');
      deviceRef.current = null;
    }
  }, []);

  const disconnect = useCallback(async () => {
    try {
      if (deviceRef.current) {
        await deviceRef.current.cancelConnection();
      }
    } catch {
      // ignore
    }
    ensureManager().stopDeviceScan();
    deviceRef.current = null;
    setConn('idle');
    setStatus('Disconnected');
  }, []);

  return (
    <SafeAreaView style={styles.container}>
      <StatusBar barStyle="light-content" />
      <Text style={styles.title}>ESP32-C3 Car</Text>
      <Text style={styles.status}>{status}</Text>

      <View style={styles.row}>
        <TouchableOpacity
          style={[
            styles.connectBtn,
            conn === 'connected' ? styles.connected : styles.connect,
          ]}
          onPress={conn === 'connected' ? disconnect : connect}
          disabled={conn === 'scanning' || conn === 'connecting'}>
          <Text style={styles.connectBtnText}>
            {conn === 'connected' ? 'Disconnect' : 'Connect'}
          </Text>
        </TouchableOpacity>
      </View>

      <View style={styles.pad}>
        <View style={styles.pairLeft}>
          <DirButton
            label="◀"
            cmd="left"
            disabled={conn !== 'connected'}
            onPress={send}
            onRelease={() => send('stop')}
          />
          <DirButton
            label="▶"
            cmd="right"
            disabled={conn !== 'connected'}
            onPress={send}
            onRelease={() => send('stop')}
          />
        </View>
        <View style={styles.pairRight}>
          <DirButton
            label="▲"
            cmd="forward"
            disabled={conn !== 'connected'}
            onPress={send}
            onRelease={() => send('stop')}
          />
          <DirButton
            label="▼"
            cmd="backward"
            disabled={conn !== 'connected'}
            onPress={send}
            onRelease={() => send('stop')}
          />
        </View>
      </View>
    </SafeAreaView>
  );
}

const BTN_SIZE = 100;

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#101418',
    alignItems: 'center',
    justifyContent: 'center',
  },
  title: {
    color: '#fff',
    fontSize: 26,
    fontWeight: '700',
    marginBottom: 4,
  },
  status: {
    color: '#9fd3ff',
    fontSize: 14,
    marginBottom: 24,
  },
  row: {flexDirection: 'row', marginBottom: 40},
  connectBtn: {
    paddingHorizontal: 28,
    paddingVertical: 12,
    borderRadius: 24,
  },
  connect: {backgroundColor: '#2f6df6'},
  connected: {backgroundColor: '#d93636'},
  connectBtnText: {color: '#fff', fontSize: 16, fontWeight: '700'},
  pad: {
    flex: 1,
    width: '100%',
    position: 'relative',
  },
  pairLeft: {
    position: 'absolute',
    left: '30%',
    top: '50%',
    transform: [{translateX: -BTN_SIZE}, {translateY: -BTN_SIZE / 2}],
    flexDirection: 'row',
    width: BTN_SIZE * 2,
    justifyContent: 'space-between',
  },
  pairRight: {
    position: 'absolute',
    left: '70%',
    top: '50%',
    transform: [{translateX: -BTN_SIZE / 2}, {translateY: -BTN_SIZE}],
    flexDirection: 'column',
    height: BTN_SIZE * 2,
    justifyContent: 'space-between',
  },
  btn: {
    width: BTN_SIZE,
    height: BTN_SIZE,
    borderRadius: BTN_SIZE / 2,
    backgroundColor: '#1f6feb',
    alignItems: 'center',
    justifyContent: 'center',
  },
  btnText: {color: '#fff', fontSize: 38, fontWeight: '700'},
  btnPressed: {backgroundColor: '#0f4fb8'},
});
