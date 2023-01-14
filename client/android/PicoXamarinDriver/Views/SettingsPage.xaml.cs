using PicoXamarinDriver.Models;
using PicoXamarinDriver.ViewModels;
using PicoXamarinDriver.Views;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace PicoXamarinDriver.Views
{
    public partial class ItemsPage : ContentPage
    {
        SettingsViewModel _viewModel;

        public ItemsPage()
        {
            InitializeComponent();

            BindingContext = _viewModel = new SettingsViewModel();
        }

        protected override void OnAppearing()
        {
            base.OnAppearing();
           // _viewModel.OnAppearing();
        }

        private void Switch_Toggled(object sender, ToggledEventArgs e)
        {

        }
    }
}