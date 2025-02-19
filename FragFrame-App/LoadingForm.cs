using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FragFrame
{
    public partial class LoadingForm : Form
    {
        public int TextIndex = 0;
        public string[] Texts =
        {
            "Striking Counters...",
            "Clearing Lower...",
            "Executing mid to B...",
            "Planting in the open...",
            "Smoking tunnels...",
            "Mollying window...",
            "Wiffing bullets...",
            "Throwing the match..."
        };

        public LoadingForm()
        {
            InitializeComponent();
            this.TopMost = true;
        }

        private void LoadingForm_Load(object sender, EventArgs e)
        {
            CustomProgressBar progressBar = new();
            progressBar.Style = ProgressBarStyle.Marquee;
            progressBar.BackColor = this.BackColor;
            progressBar.Dock = DockStyle.Fill;
            ProgressBarContainer.Controls.Add(progressBar);

            System.Windows.Forms.Timer timer = new();
            timer.Interval = 3500;
            timer.Tick += (o, e) =>
            {
                TextIndex++;
                if (TextIndex >= Texts.Length)
                    TextIndex = 0;
                this.Invoke(() =>
                {
                    label2.Text = Texts[TextIndex];
                });
            };
            timer.Start();
        }

    }
}
public class CustomProgressBar : ProgressBar
{
    private System.Windows.Forms.Timer marqueeTimer;
    private int marqueePosition;

    public CustomProgressBar()
    {
        // Enable double buffering to reduce flicker
        SetStyle(ControlStyles.OptimizedDoubleBuffer | ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint, true);

        // Initialize marquee animation
        marqueeTimer = new System.Windows.Forms.Timer();
        marqueeTimer.Interval = 1; // Adjust speed as needed
        marqueeTimer.Tick += MarqueeTimer_Tick;
    }

    protected override void OnPaint(PaintEventArgs e)
    {
        // Draw the background
        using (var brush = new SolidBrush(BackColor))
        {
            e.Graphics.FillRectangle(brush, ClientRectangle);
        }

        // Draw the marquee animation
        if (Style == ProgressBarStyle.Marquee)
        {
            int marqueeWidth = Width / 3; // Width of the marquee block
            using (var brush = new SolidBrush(ForeColor))
            {
                Region region = Region.FromHrgn(FragFrame.GDI32.CreateRoundRectRgn(marqueePosition, 0, marqueePosition + marqueeWidth, Height, 8, 8));
                e.Graphics.FillRegion(brush, region);
            }
        }
    }

    private void MarqueeTimer_Tick(object sender, EventArgs e)
    {
        // Update marquee position
        marqueePosition += 12; // Adjust speed as needed
        if (marqueePosition > Width)
        {
            marqueePosition = -Width / 3; // Reset position to create a continuous loop
        }

        // Redraw the control
        Invalidate();
    }

    protected override void OnVisibleChanged(EventArgs e)
    {
        base.OnVisibleChanged(e);

        // Start or stop the marquee timer based on visibility
        if (Visible && Style == ProgressBarStyle.Marquee)
        {
            marqueeTimer.Start();
        }
        else
        {
            marqueeTimer.Stop();
        }
    }

    protected override void OnStyleChanged(EventArgs e)
    {
        base.OnStyleChanged(e);

        // Start or stop the marquee timer based on the style
        if (Style == ProgressBarStyle.Marquee)
        {
            marqueeTimer.Start();
        }
        else
        {
            marqueeTimer.Stop();
        }
    }
}
