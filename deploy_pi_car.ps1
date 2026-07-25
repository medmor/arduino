# Deploy the pi_car_voice project to the Raspberry Pi.
$ErrorActionPreference = "Stop"

$piUser = "medmor"
$piHost = "192.168.1.7"
$piAddress = "$piUser@$piHost"
$localPath = Join-Path $PWD "pi_car_voice"
$remotePath = "~/pi_car_voice"

if (-not (Test-Path $localPath)) {
    Write-Error "Local project folder not found: $localPath"
    exit 1
}

Write-Host "Deploying pi_car_voice to $piAddress ..."
ssh $piAddress "mkdir -p $remotePath"

# Copy all files excluding Python caches if any
$files = Get-ChildItem -Path $localPath -File
foreach ($file in $files) {
    $source = $file.FullName
    $dest = "$piAddress`:$remotePath/"
    scp $source $dest
}

Write-Host "Deployment complete."
Write-Host "Next, SSH into the Pi and run: bash $remotePath/setup.sh"
Write-Host "  ssh $piAddress"
