import type { CSSProperties, ReactNode } from 'react';
import type { DesignSystem, Page, SlideMeta, SlideTransition } from '@open-slide/core';
import { ImagePlaceholder, useSlidePageNumber } from '@open-slide/core';

import guiConsole from '@assets/adjustment_gui_console.png';
import quadcopter from '@assets/BETAFPV Cetus Pro Quadcopter Sans Balais Avec 3.jpg';
import soundDistance from '@assets/distance_derived_from_sound.png';
import mpu6050 from '@assets/MPU6050.jpg';
import bicopter from '@assets/my_drone_without_propeller.jpeg';
import stm32Board from '@assets/STM32F411CEU6.jpg';
import cubeIde from '@assets/STM32CubeIDE.png';
import simulatorPhoto from '@assets/TRYP_FPV_simulator.jpg';
import ultrasonicSensor from '@assets/Ultrasonic_sensor.jpg';

export const design: DesignSystem = {
  palette: {
    bg: '#f6f1e8',
    text: '#15161a',
    accent: '#e0472f',
  },
  fonts: {
    display: '"Aptos Display", "Segoe UI", system-ui, sans-serif',
    body: '"Aptos", "Segoe UI", system-ui, sans-serif',
  },
  typeScale: {
    hero: 144,
    body: 36,
  },
  radius: 8,
};

const colors = {
  paper: '#f6f1e8',
  paperAlt: '#fff9ef',
  ink: '#15161a',
  muted: '#66676d',
  line: 'rgba(21,22,26,0.14)',
  charcoal: '#20242b',
  orange: '#e0472f',
  blue: '#2176ae',
  yellow: '#f0b43d',
  green: '#2e7d5b',
  white: '#fffdf7',
};

const font = {
  display: 'var(--osd-font-display)',
  body: 'var(--osd-font-body)',
  mono: '"Cascadia Mono", "SF Mono", Consolas, monospace',
};

const fill: CSSProperties = {
  width: '100%',
  height: '100%',
  position: 'relative',
  overflow: 'hidden',
  background: 'var(--osd-bg)',
  color: 'var(--osd-text)',
  fontFamily: font.body,
};

const styles = `
  @keyframes pdj-rise {
    from { opacity: 0; transform: translateY(18px); }
    to { opacity: 1; transform: translateY(0); }
  }
  @keyframes pdj-drift {
    0%, 100% { transform: translateY(0) rotate(-1deg); }
    50% { transform: translateY(-10px) rotate(1deg); }
  }
  .pdj-rise { opacity: 0; animation: pdj-rise .7s cubic-bezier(.2,.7,.2,1) forwards; }
  .pdj-drift { animation: pdj-drift 5s ease-in-out infinite; }
`;

const Styles = () => <style>{styles}</style>;

export const transition: SlideTransition = {
  duration: 220,
  exit: {
    duration: 140,
    easing: 'cubic-bezier(0.4, 0, 1, 1)',
    keyframes: [
      { opacity: 1, transform: 'translateY(0)' },
      { opacity: 0, transform: 'translateY(-4px)' },
    ],
  },
  enter: {
    duration: 220,
    delay: 70,
    easing: 'cubic-bezier(0, 0, 0.2, 1)',
    keyframes: [
      { opacity: 0, transform: 'translateY(8px)' },
      { opacity: 1, transform: 'translateY(0)' },
    ],
  },
};

const Footer = ({ section }: { section: string }) => {
  const { current, total } = useSlidePageNumber();
  return (
    <div
      style={{
        position: 'absolute',
        left: 120,
        right: 120,
        bottom: 54,
        display: 'flex',
        justifyContent: 'space-between',
        alignItems: 'center',
        fontFamily: font.mono,
        fontSize: 21,
        color: colors.muted,
        letterSpacing: '0.02em',
      }}
    >
      <span>{section}</span>
      <span>
        {String(current).padStart(2, '0')} / {String(total).padStart(2, '0')}
      </span>
    </div>
  );
};

const PageShell = ({
  eyebrow,
  title,
  children,
  section = 'IPSA Paris Drone Program',
  tone = 'light',
}: {
  eyebrow: string;
  title: ReactNode;
  children: ReactNode;
  section?: string;
  tone?: 'light' | 'dark';
}) => (
  <section
    style={{
      ...fill,
      background: tone === 'dark' ? colors.charcoal : 'var(--osd-bg)',
      color: tone === 'dark' ? colors.white : 'var(--osd-text)',
      padding: '94px 120px 126px',
    }}
  >
    <Styles />
    <div
      style={{
        position: 'absolute',
        inset: 0,
        backgroundImage:
          tone === 'dark'
            ? 'linear-gradient(rgba(255,255,255,.055) 1px, transparent 1px), linear-gradient(90deg, rgba(255,255,255,.055) 1px, transparent 1px)'
            : 'linear-gradient(rgba(21,22,26,.055) 1px, transparent 1px), linear-gradient(90deg, rgba(21,22,26,.055) 1px, transparent 1px)',
        backgroundSize: '64px 64px',
        opacity: 0.42,
      }}
    />
    <div style={{ position: 'relative', zIndex: 1, height: '100%', display: 'flex', flexDirection: 'column' }}>
      <div className="pdj-rise" style={{ animationDelay: '.05s' }}>
        <div
          style={{
            fontFamily: font.mono,
            fontSize: 23,
            letterSpacing: '0.14em',
            textTransform: 'uppercase',
            color: tone === 'dark' ? '#f0b43d' : 'var(--osd-accent)',
            marginBottom: 22,
          }}
        >
          {eyebrow}
        </div>
        <h2
          style={{
            margin: 0,
            fontFamily: font.display,
            fontSize: 78,
            lineHeight: 1.03,
            letterSpacing: 0,
            fontWeight: 850,
            maxWidth: 1420,
          }}
        >
          {title}
        </h2>
      </div>
      <div style={{ flex: 1, minHeight: 0 }}>{children}</div>
    </div>
    <Footer section={section} />
  </section>
);

const Photo = ({
  src,
  alt,
  style,
  delay = '.15s',
}: {
  src: string;
  alt: string;
  style?: CSSProperties;
  delay?: string;
}) => (
  <div
    className="pdj-rise"
    style={{
      animationDelay: delay,
      overflow: 'hidden',
      borderRadius: 'var(--osd-radius)',
      border: `1px solid ${colors.line}`,
      boxShadow: '0 24px 54px rgba(21,22,26,.16)',
      background: colors.paperAlt,
      ...style,
    }}
  >
    <img src={src} alt={alt} style={{ width: '100%', height: '100%', objectFit: 'cover', display: 'block' }} />
  </div>
);

const PlaceholderPhoto = ({
  hint,
  style,
  delay = '.2s',
}: {
  hint: string;
  style?: CSSProperties;
  delay?: string;
}) => (
  <div
    className="pdj-rise"
    style={{
      animationDelay: delay,
      overflow: 'hidden',
      borderRadius: 'var(--osd-radius)',
      border: `1px dashed ${colors.line}`,
      background: colors.paperAlt,
      display: 'flex',
      alignItems: 'center',
      justifyContent: 'center',
      ...style,
    }}
  >
    <ImagePlaceholder hint={hint} width={520} height={320} />
  </div>
);

const Caption = ({ children, color = colors.muted }: { children: ReactNode; color?: string }) => (
  <p style={{ margin: 0, color, fontSize: 30, lineHeight: 1.45, maxWidth: 920 }}>{children}</p>
);

const Quote = ({ children, dark = false }: { children: ReactNode; dark?: boolean }) => (
  <div
    style={{
      marginTop: 38,
      padding: '28px 34px',
      borderLeft: `8px solid ${dark ? colors.yellow : 'var(--osd-accent)'}`,
      background: dark ? 'rgba(255,255,255,.07)' : colors.paperAlt,
      fontSize: 35,
      lineHeight: 1.35,
      color: dark ? colors.white : colors.ink,
      maxWidth: 980,
    }}
  >
    {children}
  </div>
);

const TimelineCard = ({
  label,
  title,
  line1,
  line2,
  accent,
  delay,
}: {
  label: string;
  title: string;
  line1: string;
  line2: string;
  accent: string;
  delay: string;
}) => (
  <div
    className="pdj-rise"
    style={{
      animationDelay: delay,
      flex: 1,
      padding: 36,
      background: colors.white,
      border: `1px solid ${colors.line}`,
      borderRadius: 'var(--osd-radius)',
      boxShadow: '0 22px 46px rgba(21,22,26,.10)',
    }}
  >
    <div style={{ fontFamily: font.mono, fontSize: 22, color: accent, marginBottom: 24 }}>{label}</div>
    <h3 style={{ margin: 0, fontSize: 48, lineHeight: 1.08, fontWeight: 850 }}>{title}</h3>
    <p style={{ margin: '28px 0 0', fontSize: 31, lineHeight: 1.45, color: colors.muted }}>{line1}</p>
    <p style={{ margin: '10px 0 0', fontSize: 31, lineHeight: 1.45, color: colors.muted }}>{line2}</p>
  </div>
);

const AxisChip = ({ label, value, color }: { label: string; value: string; color: string }) => (
  <div style={{ padding: '24px 28px', borderRadius: 8, background: colors.white, border: `1px solid ${colors.line}` }}>
    <div style={{ fontFamily: font.mono, fontSize: 22, color, marginBottom: 8 }}>{label}</div>
    <div style={{ fontSize: 34, fontWeight: 780 }}>{value}</div>
  </div>
);

const CompareColumn = ({
  title,
  line1,
  line2,
  line3,
  accent,
}: {
  title: string;
  line1: string;
  line2: string;
  line3: string;
  accent: string;
}) => (
  <div style={{ flex: 1, padding: 38, background: colors.white, border: `1px solid ${colors.line}`, borderRadius: 8 }}>
    <h3 style={{ margin: 0, fontSize: 50, color: accent }}>{title}</h3>
    <p style={{ margin: '28px 0 0', fontSize: 32, lineHeight: 1.45, color: colors.muted }}>{line1}</p>
    <p style={{ margin: '14px 0 0', fontSize: 32, lineHeight: 1.45, color: colors.muted }}>{line2}</p>
    <p style={{ margin: '14px 0 0', fontSize: 32, lineHeight: 1.45, color: colors.muted }}>{line3}</p>
  </div>
);

const RoleCard = ({
  image,
  title,
  role,
  metaphor,
  accent,
}: {
  image: string;
  title: string;
  role: string;
  metaphor: string;
  accent: string;
}) => (
  <div style={{ flex: 1, background: colors.white, border: `1px solid ${colors.line}`, borderRadius: 8, overflow: 'hidden' }}>
    <img src={image} alt={title} style={{ width: '100%', height: 210, objectFit: 'cover', display: 'block' }} />
    <div style={{ padding: 26 }}>
      <div style={{ fontFamily: font.mono, fontSize: 21, color: accent, marginBottom: 12 }}>{title}</div>
      <div style={{ fontSize: 31, fontWeight: 800, lineHeight: 1.15 }}>{role}</div>
      <div style={{ marginTop: 14, fontSize: 26, lineHeight: 1.35, color: colors.muted }}>{metaphor}</div>
    </div>
  </div>
);

const FlowStep = ({ label, accent }: { label: string; accent: string }) => (
  <div
    style={{
      width: 184,
      height: 92,
      borderRadius: 8,
      background: colors.white,
      border: `2px solid ${accent}`,
      display: 'flex',
      alignItems: 'center',
      justifyContent: 'center',
      fontFamily: font.mono,
      fontSize: 25,
      color: colors.ink,
      fontWeight: 700,
    }}
  >
    {label}
  </div>
);

const ProjectRow = ({
  name,
  lesson,
  accent,
}: {
  name: string;
  lesson: string;
  accent: string;
}) => (
  <div style={{ display: 'grid', gridTemplateColumns: '360px 1fr', gap: 28, alignItems: 'center' }}>
    <div style={{ fontFamily: font.mono, fontSize: 25, color: accent }}>{name}</div>
    <div style={{ fontSize: 30, lineHeight: 1.35, color: colors.ink }}>{lesson}</div>
  </div>
);

const Takeaway = ({
  number,
  title,
  body,
  accent,
}: {
  number: string;
  title: string;
  body: string;
  accent: string;
}) => (
  <div style={{ flex: 1, padding: 38, borderRadius: 8, background: colors.white, border: `1px solid ${colors.line}` }}>
    <div style={{ fontFamily: font.mono, color: accent, fontSize: 28, marginBottom: 22 }}>{number}</div>
    <h3 style={{ margin: 0, fontSize: 45, lineHeight: 1.08 }}>{title}</h3>
    <p style={{ margin: '24px 0 0', fontSize: 29, lineHeight: 1.42, color: colors.muted }}>{body}</p>
  </div>
);

const Cover: Page = () => (
  <section style={{ ...fill, background: colors.charcoal, color: colors.white }}>
    <Styles />
    <Photo src={bicopter} alt="Bicopter prototype" style={{ position: 'absolute', inset: 0, border: 0, borderRadius: 0, boxShadow: 'none' }} delay="0s" />
    <div style={{ position: 'absolute', inset: 0, background: 'linear-gradient(90deg, rgba(32,36,43,.92) 0%, rgba(32,36,43,.72) 48%, rgba(32,36,43,.28) 100%)' }} />
    <div style={{ position: 'relative', zIndex: 1, height: '100%', padding: '110px 120px', display: 'flex', flexDirection: 'column', justifyContent: 'space-between' }}>
      <div className="pdj-rise" style={{ animationDelay: '.05s', fontFamily: font.mono, fontSize: 25, color: colors.yellow, letterSpacing: '0.13em', textTransform: 'uppercase' }}>
        IPSA Paris Drone Program
      </div>
      <div>
        <h1 className="pdj-rise" style={{ animationDelay: '.15s', margin: 0, fontFamily: font.display, fontSize: 'var(--osd-size-hero)', lineHeight: .95, letterSpacing: 0, maxWidth: 1240 }}>
          Two Weeks in Paris:
          <br />
          Drones, Friends,
          <br />
          and Getting Lost
        </h1>
        <Quote dark>
          "I came to learn drones. I also learned that Google Maps in Paris is only a suggestion."
        </Quote>
      </div>
      <div className="pdj-rise" style={{ animationDelay: '.45s', fontSize: 30, color: '#e8e1d6' }}>
        From not knowing how a drone stays calm, to building a bicopter I could tune.
      </div>
    </div>
  </section>
);

const Overview: Page = () => (
  <PageShell eyebrow="Opening" title="This was not just a class. It was a two-week experiment.">
    <div style={{ marginTop: 64, display: 'flex', gap: 32 }}>
      <TimelineCard label="Week 1" title="Learning how drones think" line1="Thrust, roll, pitch, yaw." line2="Simulator first, real drone later." accent={colors.blue} delay=".12s" />
      <TimelineCard label="Week 2" title="Building the bicopter" line1="STM32, sensors, motors." line2="C code, USB, GUI, tuning." accent={colors.orange} delay=".22s" />
      <TimelineCard label="After Class" title="Paris became part of the course" line1="Eiffel Tower, fireworks, Seine." line2="Friends, cooking, parties, traffic." accent={colors.green} delay=".32s" />
    </div>
    <Quote>
      The story is simple: we learned by building, failing, tuning, traveling, and sharing the journey with people.
    </Quote>
  </PageShell>
);

const DroneBasics: Page = () => (
  <PageShell eyebrow="Part 1" title="At first, a drone looked calm. Then I learned it is correcting itself all the time.">
    <div style={{ marginTop: 58, display: 'grid', gridTemplateColumns: '1fr 1.05fr', gap: 46, alignItems: 'stretch' }}>
      <Photo src={quadcopter} alt="Training quadcopter" style={{ height: 520 }} />
      <div className="pdj-rise" style={{ animationDelay: '.2s', display: 'flex', flexDirection: 'column', gap: 18, justifyContent: 'center' }}>
        <AxisChip label="ROLL" value="left and right tilt" color={colors.orange} />
        <AxisChip label="PITCH" value="forward and backward tilt" color={colors.blue} />
        <AxisChip label="YAW" value="turning left or right" color={colors.green} />
      </div>
    </div>
    <Quote>
      A drone looks relaxed, but every second it is asking: "Am I tilted? Should I correct?"
    </Quote>
  </PageShell>
);

const SimulatorToReality: Page = () => (
  <PageShell eyebrow="Part 2" title="The simulator was where mistakes were cheap. The real drone made them loud.">
    <div style={{ marginTop: 58, display: 'flex', gap: 34, alignItems: 'stretch' }}>
      <CompareColumn title="Simulator" line1="Crash means restart." line2="Errors are safe." line3="The model waits patiently." accent={colors.blue} />
      <CompareColumn title="Real Drone" line1="Crash has sound." line2="Errors have weight." line3="Everyone turns around." accent={colors.orange} />
      <div style={{ flex: 1.1, display: 'flex', flexDirection: 'column', gap: 20 }}>
        <Photo src={simulatorPhoto} alt="FPV simulator setup" style={{ height: 256 }} delay=".25s" />
        <Photo src={quadcopter} alt="Real drone practice" style={{ height: 256 }} delay=".35s" />
      </div>
    </div>
    <Quote>
      In the simulator, crash was a screen. In real life, crash was the moment everyone looked at you.
    </Quote>
  </PageShell>
);

const PartsAsCharacters: Page = () => (
  <PageShell eyebrow="Part 3" title="Before the bicopter could fly, we had to meet its organs.">
    <div style={{ marginTop: 56, display: 'flex', gap: 24 }}>
      <RoleCard image={stm32Board} title="STM32" role="the small decision maker" metaphor="Reads, decides, outputs." accent={colors.blue} />
      <RoleCard image={ultrasonicSensor} title="Ultrasonic" role="the height question" metaphor="Asks the floor: how far?" accent={colors.green} />
      <RoleCard image={mpu6050} title="IMU" role="the balance sense" metaphor="Feels tilt and movement." accent={colors.orange} />
      <RoleCard image={soundDistance} title="C code" role="the behavior rules" metaphor="Turns data into action." accent={colors.yellow} />
    </div>
    <Caption color={colors.ink}>
      It was easier to understand the machine when each part had a job: brain, distance sense, balance sense, and muscle control.
    </Caption>
  </PageShell>
);

const Assembly: Page = () => (
  <PageShell eyebrow="Part 4" title="Building the bicopter was the day the parts started arguing with each other.">
    <div style={{ marginTop: 56, display: 'grid', gridTemplateColumns: '1.25fr .95fr', gap: 44, alignItems: 'center' }}>
      <Photo src={bicopter} alt="Bicopter without propellers" style={{ height: 560 }} />
      <div className="pdj-rise" style={{ animationDelay: '.2s' }}>
        <Caption color={colors.ink}>A bicopter has fewer motors than a quadcopter, so small changes matter more.</Caption>
        <div style={{ height: 34 }} />
        <Caption>It was not "assemble it and fly." It was sensor data, motor response, code, and parameters learning to work together.</Caption>
        <Quote>
          I thought assembly was the finish line. Actually, it was just the first day the bicopter started debating with me.
        </Quote>
      </div>
    </div>
  </PageShell>
);

const CodeToMovement: Page = () => (
  <PageShell eyebrow="Part 5" title="The most surprising part: changing C code changed a real machine.">
    <div style={{ marginTop: 54, display: 'grid', gridTemplateColumns: '1fr 1.05fr', gap: 42, alignItems: 'center' }}>
      <Photo src={guiConsole} alt="Bicopter adjustment GUI console" style={{ height: 536 }} />
      <div>
        <div className="pdj-rise" style={{ animationDelay: '.2s', display: 'flex', gap: 12, alignItems: 'center', flexWrap: 'wrap' }}>
          <FlowStep label="Code" accent={colors.blue} />
          <div style={{ fontSize: 34, color: colors.muted }}>{'->'}</div>
          <FlowStep label="Upload" accent={colors.green} />
          <div style={{ fontSize: 34, color: colors.muted }}>{'->'}</div>
          <FlowStep label="Observe" accent={colors.orange} />
          <div style={{ fontSize: 34, color: colors.muted }}>{'->'}</div>
          <FlowStep label="Tune" accent={colors.yellow} />
          <div style={{ fontSize: 34, color: colors.muted }}>{'->'}</div>
          <FlowStep label="Repeat" accent={colors.blue} />
        </div>
        <Quote>
          The achievement was not perfect flight. It was starting to understand why it was unstable, and what I could adjust next.
        </Quote>
      </div>
    </div>
  </PageShell>
);

const WorkspaceProjects: Page = () => (
  <PageShell eyebrow="Projects completed" title="The final prototype came from a chain of smaller wins.">
    <div style={{ marginTop: 48, display: 'grid', gridTemplateColumns: '1.02fr .98fr', gap: 44, alignItems: 'stretch' }}>
      <div className="pdj-rise" style={{ animationDelay: '.15s', background: colors.white, border: `1px solid ${colors.line}`, borderRadius: 8, padding: 34, display: 'flex', flexDirection: 'column', gap: 22 }}>
        <ProjectRow name="LED_Blinking" lesson="First proof that code could reach the STM32 board." accent={colors.muted} />
        <ProjectRow name="BP_UltrasonicSensor" lesson="Measured distance and turned echo time into altitude clues." accent={colors.green} />
        <ProjectRow name="BP_IMU" lesson="Read MPU6050 data and estimated roll and pitch with filtering." accent={colors.blue} />
        <ProjectRow name="BP_OneShot125" lesson="Sent motor commands through ESC pulse widths." accent={colors.yellow} />
        <ProjectRow name="BP_Stabilization" lesson="Combined sensors, PID, mixer, motors, and USB tuning." accent={colors.orange} />
        <ProjectRow name="python_scripts" lesson="Built a PC-side interface for COM ports, gains, takeoff, land, and target altitude." accent={colors.green} />
      </div>
      <div style={{ display: 'grid', gridTemplateRows: '1fr 1fr', gap: 22 }}>
        <Photo src={cubeIde} alt="STM32CubeIDE" style={{ height: '100%' }} delay=".25s" />
        <Photo src={stm32Board} alt="STM32F411 board" style={{ height: '100%' }} delay=".35s" />
      </div>
    </div>
  </PageShell>
);

const ParisOutside: Page = () => (
  <PageShell eyebrow="Part 6" title="Paris was not just the background. It became part of the course.">
    <div style={{ marginTop: 54, display: 'grid', gridTemplateColumns: '1fr 1fr 1fr', gap: 26 }}>
      <PlaceholderPhoto hint="Eiffel Tower or Bastille Day fireworks photo" style={{ height: 360 }} delay=".12s" />
      <PlaceholderPhoto hint="Montmartre, Notre-Dame, or Seine river cruise photo" style={{ height: 360 }} delay=".22s" />
      <PlaceholderPhoto hint="A favorite Paris street or night view from the trip" style={{ height: 360 }} delay=".32s" />
    </div>
    <div className="pdj-rise" style={{ animationDelay: '.42s', marginTop: 42, display: 'grid', gridTemplateColumns: '1.2fr .8fr', gap: 40, alignItems: 'center' }}>
      <Caption color={colors.ink}>
        After class, the city changed the rhythm: Eiffel Tower, Bastille Day fireworks, Montmartre, Seine cruise, Notre-Dame, and a Louvre plan defeated by the heatwave.
      </Caption>
      <Quote>
        Louvre was on the plan, but the heatwave had a stronger argument.
      </Quote>
    </div>
  </PageShell>
);

const FriendsTraffic: Page = () => (
  <PageShell eyebrow="Part 7" title="The best debugging sessions were not always in the lab.">
    <div style={{ marginTop: 50, display: 'grid', gridTemplateColumns: '.95fr 1.05fr', gap: 42, alignItems: 'stretch' }}>
      <div style={{ display: 'grid', gridTemplateRows: '1fr 1fr', gap: 24 }}>
        <PlaceholderPhoto hint="Friends cooking together or dinner photo" style={{ height: '100%' }} delay=".12s" />
        <PlaceholderPhoto hint="Party, classmates, or group memory photo" style={{ height: '100%' }} delay=".22s" />
      </div>
      <div className="pdj-rise" style={{ animationDelay: '.28s', background: colors.charcoal, color: colors.white, borderRadius: 8, padding: 44, display: 'flex', flexDirection: 'column', justifyContent: 'center' }}>
        <div style={{ fontFamily: font.mono, fontSize: 25, color: colors.yellow, marginBottom: 28 }}>
          Paris Transportation: real-world control system
        </div>
        <p style={{ margin: 0, fontSize: 48, lineHeight: 1.15, fontWeight: 850 }}>
          Estimated arrival time is not a promise.
        </p>
        <p style={{ margin: '28px 0 0', fontSize: 34, lineHeight: 1.45, color: '#ddd7cd' }}>
          It is a creative writing exercise with unexpected disturbances.
        </p>
        <Quote dark>
          During the day we studied control systems. At night, Paris traffic gave us field testing.
        </Quote>
      </div>
    </div>
  </PageShell>
);

const Takeaways: Page = () => (
  <PageShell eyebrow="Closing" title="What I brought home was bigger than a prototype.">
    <div style={{ marginTop: 72, display: 'flex', gap: 28 }}>
      <Takeaway number="01" title="Learning by building is different" body="Sensors, code, and mechanics became one system I could touch." accent={colors.blue} />
      <Takeaway number="02" title="Engineering is iteration" body="Test, observe, tune, repeat. Success came from understanding the next adjustment." accent={colors.orange} />
      <Takeaway number="03" title="The best part was people" body="The course, the city, and the friends turned two weeks into a real memory." accent={colors.green} />
    </div>
    <Quote>
      I came to Paris to learn about drones. I left with a bicopter prototype, new friends, and a more humble attitude toward public transportation.
    </Quote>
  </PageShell>
);

const ThankYou: Page = () => (
  <section style={{ ...fill, background: colors.charcoal, color: colors.white }}>
    <Styles />
    <div style={{ position: 'absolute', inset: 0, backgroundImage: 'linear-gradient(rgba(255,255,255,.055) 1px, transparent 1px), linear-gradient(90deg, rgba(255,255,255,.055) 1px, transparent 1px)', backgroundSize: '64px 64px', opacity: .45 }} />
    <Photo src={bicopter} alt="Bicopter prototype" style={{ position: 'absolute', right: 120, top: 120, width: 610, height: 720, transform: 'rotate(2deg)', borderColor: 'rgba(255,255,255,.18)' }} delay=".1s" />
    <div style={{ position: 'relative', zIndex: 1, height: '100%', padding: '140px 120px', display: 'flex', flexDirection: 'column', justifyContent: 'center', maxWidth: 1130 }}>
      <div className="pdj-rise" style={{ animationDelay: '.05s', fontFamily: font.mono, fontSize: 26, letterSpacing: '0.14em', textTransform: 'uppercase', color: colors.yellow, marginBottom: 34 }}>
        Thank you
      </div>
      <h2 className="pdj-rise" style={{ animationDelay: '.18s', margin: 0, fontFamily: font.display, fontSize: 132, lineHeight: .98, letterSpacing: 0 }}>
        Questions?
      </h2>
      <p className="pdj-rise" style={{ animationDelay: '.32s', margin: '42px 0 0', fontSize: 38, lineHeight: 1.35, color: '#e8e1d6', maxWidth: 820 }}>
        From not knowing how it flies, to knowing how to tune what makes it unstable.
      </p>
    </div>
  </section>
);

export const meta: SlideMeta = {
  title: 'Two Weeks in Paris: Drones, Friends, and Getting Lost',
  createdAt: '2026-07-16T13:33:20.869Z',
};

export default [
  Cover,
  Overview,
  DroneBasics,
  SimulatorToReality,
  PartsAsCharacters,
  Assembly,
  CodeToMovement,
  WorkspaceProjects,
  ParisOutside,
  FriendsTraffic,
  Takeaways,
  ThankYou,
] satisfies Page[];
