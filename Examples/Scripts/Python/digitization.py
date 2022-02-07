#!/usr/bin/env python3
from pathlib import Path
from typing import Optional, Union

import acts
import acts.examples


u = acts.UnitConstants

def addDigitization(
    s: acts.examples.Sequencer,
    trackingGeometry: acts.TrackingGeometry,
    field: acts.MagneticFieldProvider,
    digiConfigFile: Union[Path, str],
    outputDirCsv: Optional[Union[Path, str]] = None,
    outputDirRoot: Optional[Union[Path, str]] = None,
    rnd: Optional[acts.examples.RandomNumbers] = None,
) -> acts.examples.Sequencer:

    # Preliminaries
    rnd = rnd or acts.examples.RandomNumbers()

    # Digitization
    digiCfg = acts.examples.DigitizationConfig(
        acts.examples.readDigiConfigFromJson(
            str(Path(digiConfigFile)),
        ),
        trackingGeometry=trackingGeometry,
        randomNumbers=rnd,
        inputSimHits="simhits",
    )
    digiAlg = acts.examples.DigitizationAlgorithm(digiCfg, s.config.logLevel)

    s.addAlgorithm(digiAlg)

    if outputDirRoot is not None:
        outputDirRoot = Path(outputDirRoot)
        if not outputDirRoot.exists():
            outputDirRoot.mkdir()
        rmwConfig = acts.examples.RootMeasurementWriter.Config(
            inputMeasurements=digiAlg.config.outputMeasurements,
            inputClusters=digiAlg.config.outputClusters,
            inputSimHits=digiAlg.config.inputSimHits,
            inputMeasurementSimHitsMap=digiAlg.config.outputMeasurementSimHitsMap,
            filePath=str(outputDirRoot / f"{digiAlg.config.outputMeasurements}.root"),
            trackingGeometry=trackingGeometry,
        )
        rmwConfig.addBoundIndicesFromDigiConfig(digiAlg.config)
        s.addWriter(acts.examples.RootMeasurementWriter(rmwConfig, s.config.logLevel))

    if outputDirCsv is not None:
        outputDirCsv = Path(outputDirCsv)
        if not outputDirCsv.exists():
            outputDirCsv.mkdir()
        s.addWriter(
            acts.examples.CsvMeasurementWriter(
                level=s.config.logLevel,
                inputMeasurements=digiAlg.config.outputMeasurements,
                inputClusters=digiAlg.config.outputClusters,
                inputSimHits=digiAlg.config.inputSimHits,
                inputMeasurementSimHitsMap=digiAlg.config.outputMeasurementSimHitsMap,
                outputDir=str(outputDirCsv),
            )
        )

    return s


def configureDigitization(
    trackingGeometry,
    field,
    outputDir: Path,
    particlesInput: Optional[Path] = None,
    outputRoot=True,
    outputCsv=True,
    s=None,
) -> acts.examples.Sequencer:

    from particle_gun import addParticleGun, EtaConfig, PhiConfig, ParticleConfig
    from fatras import addFatras

    s = s or acts.examples.Sequencer(events=100, numThreads=-1, logLevel = acts.logging.INFO)
    rnd = acts.examples.RandomNumbers(seed=42)

    if particlesInput is None:
        s = addParticleGun(
            s,
            EtaConfig(-2.0, 2.0),
            ParticleConfig(4, acts.PdgParticle.eMuon, True),
            PhiConfig(0.0, 360.0 * u.degree),
            multiplicity=2,
            rnd=rnd,
        )
    else:
        # Read input from input collection (e.g. Pythia8 output)
        evGen = acts.examples.RootParticleReader(
            level=s.config.logLevel,
            particleCollection="particles_input",
            filePath=str(particlesInput),
            orderedEvents=False,
        )
        s.addReader(evGen)

    outputDir = Path(outputDir)
    s = addFatras(
        s,
        trackingGeometry,
        field,
        rnd=rnd,
    )

    s = addDigitization(
        s,
        trackingGeometry,
        field,
        digiConfigFile=Path(__file__).resolve().parent.parent.parent.parent
        / "Examples/Algorithms/Digitization/share/default-smearing-config-generic.json",
        outputDirCsv=outputDir / "csv" if outputCsv else None,
        outputDirRoot=outputDir if outputRoot else None,
        rnd=rnd,
    )

    return s


if "__main__" == __name__:
    detector, trackingGeometry, _ = acts.examples.GenericDetector.create()

    field = acts.ConstantBField(acts.Vector3(0, 0, 2 * u.T))

    configureDigitization(trackingGeometry, field, outputDir=Path.cwd()).run()
